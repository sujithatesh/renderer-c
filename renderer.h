/* date = June 29th 2026 1:46 pm */

#ifndef RENDERER_H
#define RENDERER_H

// 0xAARRBBGG
typedef enum COLORS
{
	WHITE=0xFFFFFFFF,
	PRIMARY_YELLOW=0xFFFF8800,
	GREY = 0xFF888888,
	RED=0xFFFF0000,
	BLACK=0xFF000000,
	BLUE=0xFF0000FF
} COLORS;

typedef struct PIXEL_BUFFER PIXEL_BUFFER;
struct PIXEL_BUFFER
{
	int32_t width, height;
	uint32_t *pixels;
};

internal void r_draw_point(PIXEL_BUFFER* px_buf, v2 point1, COLORS color);
internal void r_draw_line(PIXEL_BUFFER* px_buf, v2 point1, v2 point2, uint32_t thickness, COLORS color);
internal void r_draw_line_br(PIXEL_BUFFER* px_buf, v2 point1, v2 point2, COLORS color);
internal void r_draw_line_normal(PIXEL_BUFFER* px_buf, v2 normal, uint32_t size, uint32_t thickness);
internal void r_fill_background(PIXEL_BUFFER *px_buf, COLORS color);
internal void r_fill_background_avx2(PIXEL_BUFFER *px_buf, COLORS color);
internal void r_draw_rectangle(PIXEL_BUFFER* px_buf, v2 bottom_left, uint32_t width, uint32_t height, COLORS color);
internal void r_draw_rectangle_avx2(PIXEL_BUFFER* px_buf, v2 bottom_left, uint32_t width, uint32_t height, COLORS color);
internal void r_cpy_bitmap_to_px_buff(PIXEL_BUFFER* px_buf, PIXEL_BUFFER* bitmap, uint32_t width, uint32_t height);

#endif //RENDERER_H
