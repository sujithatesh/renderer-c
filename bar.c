#include "math.h"
#include "renderer.c"
#include "bar.h"

internal void
populate_pixel_buffer(PIXEL_BUFFER* px_buf)
{
	r_fill_background(px_buf, GREY);
	r_draw_rectangle_outline(px_buf, V2(10, 10),px_buf->width - 20, px_buf->height - 20, WHITE);
}