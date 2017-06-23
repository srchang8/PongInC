
/*
	paddle.h designed for pong 1.0.
	Used to initialize the paddle and it has
	functions to allow user to move paddle and check if contact
	is made from the ball.
	implementations in paddle.c

*/
#ifndef HEADER_PADDLE_FILE
#define HEADER_PADDLE_FILE


void paddle_init();
void paddle_up();
void paddle_down();
int paddle_contact(int y, int x);

#endif



