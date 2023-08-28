#include "tc.h"

// Reminder :
// printf("\033[XA"); // Move up X lines;
// printf("\033[XB"); // Move down X lines;
// printf("\033[XC"); // Move right X column;
// printf("\033[XD"); // Move left X column;
// printf("\033[2J"); // Clear screen

void tc_get_cols_rows(int *cols, int *rows) {

	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*cols = size.ws_col;
	*rows = size.ws_row;

} // tc_get_cols_rows*/

void tc_echo_on(void) {

	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag |= ECHO;
	tcsetattr(1, TCSANOW, &term);

} // tc_echo_on*/

void tc_echo_off(void) {

	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag &= ~ECHO;
	tcsetattr(1, TCSANOW, &term);

} // tc_echo_off*/

void tc_icanon_on(void) {

	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag |= ICANON;
	tcsetattr(1, TCSANOW, &term);

} // tc_icanon_on*/

void tc_icanon_off(void) {

	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag &= ~ICANON;
	tcsetattr(1, TCSANOW, &term);

} // tc_icanon_off*/
