/*  pong 1.0	
 *	
 * Ping Pong game for 1 player.
 * Game is over when user runs out of balls.
 * Use 'k' to move up and 'm' for down
 * 'Q' to quit game
 *
 *	blocks on read, but timer tick sends SIGALRM caught by ball_move
 *	build:   cc pong.c set_ticker.c paddle.c -lcurses -o pong
 *  
 */

#include	<curses.h>
#include	<signal.h>
#include	<time.h>
#include 	<stdlib.h> /* srand, rand */
#include	"bounce.h"
#include 	"paddle.h"
#include 	"set_ticker.h"

static Ppball the_ball;
static time_t start_seconds;

void set_up();
void wrap_up();
void drawWalls(int startx, int starty, int horLen, int verLen);
int randomize();
void paddle_move(int signum);
void showGameStats();
void serve();
void set_up_walls();
void ball_moved();

static int top_row, bot_row, left_edge, right_edge;

int main()
{	
	int	c;
	set_up();
	serve();
	
	while ( the_ball.balls_left > 0 && ( c = getch()) != 'Q'){
		
		if ( c == 'k' ) paddle_up();
		else if ( c == 'm' ) paddle_down();
		
	}

	wrap_up();
	return 0;
}

/*
	Gets the game ready by setting up a few things
	 -setting up bal settings
	 -drawing the pong walls
	 -initializing paddle
	 -setting up alarm for ball move
	 -shows game stats
*/
void set_up(){

	void ball_move(int);
	//randomize ball inverse speed
	srand(time(NULL));
	
	the_ball.y_dir = 1  ;
	the_ball.x_dir = 1  ;
	the_ball.symbol = DFL_SYMBOL ;// 'o' is being used as the ball
	the_ball.balls_left = START_NUM_BALLS;// start off with 3

	//disable control c
	signal( SIGINT , SIG_IGN );

	initscr();
	noecho();//stops getch from echoing chars as they are being typed
	crmode();
	
	set_up_walls();
	paddle_init(top_row, bot_row, right_edge);
	
	signal( SIGALRM, ball_move );
	set_ticker( 1000 / TICKS_PER_SEC );
	
	start_seconds = time(NULL);
	showGameStats();
	refresh();
}

/*
Draw walls accordingly with the required 
edges and rows from assignment
*/
void set_up_walls(){

	left_edge = 3;
	top_row = 3;
	int y,x;
	getmaxyx(stdscr,y,x);
	right_edge = x-4;
	bot_row = y-4;
	drawWalls( left_edge, top_row, right_edge, bot_row);

}
/*
	set ball to correct positions and 
	randomize its velocity
*/
void serve(){
	
	the_ball.y_pos = top_row + (bot_row-top_row)/2;
	the_ball.x_pos = left_edge+(right_edge-left_edge)/2;
	the_ball.y_ttg = the_ball.y_ttm = randomize();
	the_ball.x_ttg = the_ball.x_ttm = randomize();
	//putt o in its position
	mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol );

}
/*
	restore cursor to bottom lower left hand corner
	arranges for the interval timer to issue SIGALRM's 
	at regular intervals
*/
void wrap_up()
{
	set_ticker( 0 );
	endwin();	
}


/*
	responsible for:
		-moving the ball at correct speed
		-shows balls and time on the screen 
*/
void ball_move(int signum){

	int	y_cur, x_cur, moved;
	signal( SIGALRM , SIG_IGN );
	showGameStats();
	y_cur = the_ball.y_pos ;		
	x_cur = the_ball.x_pos ;
	moved = 0 ;

	if ( the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1 ){
		the_ball.y_pos += the_ball.y_dir ;	/* move	*/
		the_ball.y_ttg = the_ball.y_ttm  ;	/* reset*/
		moved = 1;
	}
	if ( the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1 ){
		the_ball.x_pos += the_ball.x_dir ;	/* move	*/
		the_ball.x_ttg = the_ball.x_ttm  ;	/* reset*/
		moved = 1;
	}
	if ( moved ){
		mvaddch( y_cur, x_cur, BLANK );
		ball_moved();
	}
	if (the_ball.balls_left > 0){
		signal( SIGALRM, ball_move);
	}
}

/*
	Manages the bounce of the ball from the
	walls to the paddle, randomizes the ball velocity.
*/
int bounce_or_lose(Ppball *bp)
{
	int	return_val = 0 ;

	if ( bp->y_pos == top_row+1 ){
		bp->y_dir = 1 ; 
		return_val = 1 ;
	} else if ( bp->y_pos == bot_row-1 ){
		bp->y_dir = -1 ;
	       	return_val = 1;
	}
	if ( bp->x_pos == left_edge+1 ){
		bp->x_dir = 1 ;
	       	return_val = 1 ;
	}
	if ( paddle_contact(bp->y_pos, bp->x_pos+1) 
	|| paddle_contact(bp->y_pos+bp->y_dir, bp->x_pos+1) ){	
		bp->x_dir = -1;
		the_ball.y_ttg = the_ball.y_ttm = randomize();
		the_ball.x_ttg = the_ball.x_ttm = randomize();
	       	
	       	return_val = 1;
	} 
	else if ( bp->x_pos >= right_edge){
	       	return_val = -1;
	}
	return return_val;
}

/*
	check if the ball hit the boundaries 
	update balls left and show end game screen
*/
void ball_moved(){
		
	int result = bounce_or_lose( &the_ball );

	if (result == -1 ){
		the_ball.balls_left--;
		if (the_ball.balls_left > 0){
			serve();
		}
		else{
			showGameStats();
			mvprintw(top_row + (bot_row-top_row)/2, 
			left_edge+(right_edge-left_edge)/2, "GAME OVER");
			set_ticker( 0 );
		}	
	} 
	else{
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	}
	
	//move cursor bottom right of screen
	move(LINES-1,COLS-1);
	
	//update terminal screen
	refresh();

}



/*
	Displays the Balls left and time elapsed
	in minutes and seconds
*/
void showGameStats(){

	time_t current_seconds = time(NULL);
	int secondsPassed = (int)difftime(current_seconds, start_seconds);
	mvprintw(top_row-1, right_edge-18, "TOTAL TIME: %3d:%02d", secondsPassed/60, secondsPassed%60);
	mvprintw(top_row-1, left_edge, "BALLS LEFT: %d", the_ball.balls_left);
	
}

/*
	Display walls appropriately on screen
*/
void drawWalls(int startx, int starty, int endx, int endy){
	
	int i = 0;
	
	//top row
	for ( i = startx; i <= endx; i++ ){
		move(starty,i);
		addch('-');	
	}
	//starty+1 to avoid overwriting corner
	for ( i = starty+1; i < endy; i++ ){
		move(i,startx);
		addch('|');	
	}
	//bottom row
	for ( i = startx; i <= endx; i++ ){
		move(endy,i);
		addch('-');	
	}
}

//randomize an int from 5-10
int randomize(){
	int r = rand(), n = 7;
	return (r % n)+5;
}



