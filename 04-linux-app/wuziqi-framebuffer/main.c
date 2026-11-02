#include <stdio.h>
#include "framebuffer.h"
#include "game.h"

int main(void)
{
	init_fb("/dev/fb0");

    #if 0
	draw_clear(0xCDB79E);

	draw_point(400, 300, RED);
	
    draw_h_line(200,200, 100, YELLOW);

	
	draw_circle(400, 300, 100, RED);
#endif
	
	show_chess_broad();
    begin_game();

	return 0;
}
