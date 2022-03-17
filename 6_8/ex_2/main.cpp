#include "screen.hpp"
#include <unistd.h>


int main()
{
	screen_init();
	put_line(0, 0, 150, 150);
	put_line(150, 150, 400, 150);
	put_line(400, 150, 350, 300);
	put_line(350, 300, 150, 150);
	screen_refresh();
	for (;;);
	screen_destroy();
}

