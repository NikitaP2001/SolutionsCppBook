#include "screen.hpp"
#include <unistd.h>


int main()
{
	screen_init();
	put_circle(250, 250, 100);
	put_triangle(100, 100, 300, 100, 150, 300);
	screen_refresh();
	for (;;);
	screen_destroy();
}

