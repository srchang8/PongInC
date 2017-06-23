

/* bounce.h			*/

/* Settings for the game	*/


#ifndef HEADER_BOUNCE_FILE
#define HEADER_BOUNCE_FILE

#define	BLANK		' '
#define	DFL_SYMBOL	'o'//ball symbol
#define PAD_SYMBOL	'#'//paddle symbol
#define START_NUM_BALLS 3 //number of lives you start with
#define	TICKS_PER_SEC	50		/* affects speed	*/


/** the ping pong ball **/

typedef struct {
		int	y_pos, x_pos,
			y_ttm, x_ttm,//used for speed, setting
			y_ttg, x_ttg,//time to go?, current state
			y_dir, x_dir;//direction
		int balls_left;
		char	symbol ;

	} Ppball;


#endif