/*

	Paddle.c 
	used to initialize the paddle for pong 1.0
	It will initilize the paddle data structure and display on screen
	checks for paddle contact and allow users to move paddle

*/


#include 	<stdio.h>
#include	<curses.h>
#include	"bounce.h"
#include 	"paddle.h"


/* ping pong paddle */

typedef struct {
	
	int pad_top, pad_bot, pad_col, pad_size, top_row, bot_row;
	char pad_char;

} Pppaddle; 

Pppaddle the_paddle;

//initialize paddle data structure and draw on screen
void paddle_init(int top_row, int bot_row, int right_edge){

	the_paddle.pad_char = PAD_SYMBOL;
	int wall_height, i;
	
	wall_height = bot_row - top_row;
	
	the_paddle.pad_size = wall_height/3;
	the_paddle.pad_col = right_edge;

	the_paddle.pad_bot = (top_row + (the_paddle.pad_size + the_paddle.pad_size)-1);
	the_paddle.pad_top = top_row + the_paddle.pad_size;
	the_paddle.bot_row = bot_row;
	the_paddle.top_row = top_row;

	for (i = the_paddle.pad_top; i <= the_paddle.pad_bot; i++){
		
		mvaddch( i, the_paddle.pad_col, the_paddle.pad_char );
	}
	
	refresh();
}


/*check if paddle has reached the limits of its path and if not , to adjust the data
structure and then adjust the screen representation of the paddle */
void paddle_up(){

	if (the_paddle.pad_top > the_paddle.top_row+1 ){

		mvaddch( the_paddle.pad_bot, the_paddle.pad_col, BLANK );
		the_paddle.pad_bot--;
		the_paddle.pad_top--;
		mvaddch( the_paddle.pad_top, the_paddle.pad_col, the_paddle.pad_char );
		
	}
}
void paddle_down(){
	
	if (the_paddle.pad_bot < the_paddle.bot_row-1 ){
		
		mvaddch( the_paddle.pad_top, the_paddle.pad_col, BLANK );
		the_paddle.pad_top++;
		the_paddle.pad_bot++;
		mvaddch( the_paddle.pad_bot, the_paddle.pad_col, the_paddle.pad_char );
	}

}


/*
position of ball is passed to this function
function tells if the given position is in contact with the paddle
if yes then the calling function can choose to bounce the ball
*/
int paddle_contact(int y, int x){
	
	int i;
	//x!=pad col return 0
	//y< pad top then return 0, y > pad bot then return 0
	for (i = the_paddle.pad_top; i <= the_paddle.pad_bot; i++){
		if ( y == i && x == the_paddle.pad_col ){ return 1; }
	}

	return 0;
}








