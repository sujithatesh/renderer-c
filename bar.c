#include "math.h"
#include "renderer.c"
#include "bar.h"

internal void
populate_pixel_buffer(PIXEL_BUFFER* px_buf)
{
	r_fill_background(px_buf, BLUE);
	r_fill_background_avx2(px_buf, GREY);
	static int offset = 0;
	if((offset >= 40) == 0 && offset > -40)
	{
		offset -= 1;
	}
	else 
	{
		offset += 1;
	}
	
	r_draw_rectangle(px_buf, V2(100,100), 307, 207, PRIMARY_YELLOW);
	r_draw_rectangle_avx2(px_buf, V2(100, 100), 307, 207, WHITE);
}
