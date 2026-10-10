#include "renderer.h"
#include <immintrin.h>

internal void 
r_fill_background(PIXEL_BUFFER *px_buf, COLORS color)
{
	
	for(uint32_t i = 0; i < px_buf->width * px_buf->height; i++)
	{
		px_buf->pixels[i] = color;
	}
}

internal void 
r_fill_background_avx2(PIXEL_BUFFER *px_buf, COLORS color)
{
	
	__m256i color_vec = _mm256_set1_epi32(color);
	for(int i = 0; i  + 8 < px_buf->width * px_buf->height; i+=8)
	{
		_mm256_storeu_si256((__m256i*)(px_buf->pixels + i), color_vec);
	}
}

internal void
r_draw_point(PIXEL_BUFFER *px_buf, v2 point, COLORS color)
{
	if(point.x < 0 || point.x >= px_buf->width || point.y < 0 || point.y >= px_buf->height) return;
	
	uint32_t pixel = px_buf->width * (px_buf->height - point.y - 1) + point.x;
	px_buf->pixels[pixel] = color;
}

internal void 
r_draw_line_normal(PIXEL_BUFFER* px_buf, v2 normal, uint32_t length , uint32_t thickness)
{
	//1, 1, 9(pixels)
	
}

internal void
r_draw_rectangle(PIXEL_BUFFER* px_buf, v2 bottom_left, uint32_t width, uint32_t height, COLORS color)
{
	for(int i = bottom_left.x; i < bottom_left.x + width; i++)
	{
		for(int j = bottom_left.y; j < bottom_left.y + height; j++)
		{
			r_draw_point(px_buf, V2(i,j), color);
		}
	}
}

// TODO(sujith):  check bounds
internal void
r_draw_rectangle_avx2(PIXEL_BUFFER* px_buf, v2 bottom_left, uint32_t width, uint32_t height, COLORS color)
{
	__m256i color_vec = _mm256_set1_epi32(color);
	uint32_t top_y    = px_buf->height - bottom_left.y - height;
	for(uint32_t i = top_y; i < top_y + height; i++)
	{
		uint32_t* row_ptr        = px_buf->pixels + (i * px_buf->width);
		uint32_t remainder       = (uint32_t)(bottom_left.x + width) % 8;
		uint32_t remainder_start = bottom_left.x + width - remainder;
		for(int j = bottom_left.x; j < remainder_start; j+=8)
		{
			_mm256_storeu_si256((__m256i*)(row_ptr + j), color_vec);
		}
		for(uint32_t j = remainder_start + 1; j < remainder_start + remainder; j++)
		{
			r_draw_point(px_buf, V2(j ,(px_buf->height - i) - 1), color);
		}
	}
}

internal void
r_draw_rectangle_outline(PIXEL_BUFFER* px_buf, v2 bottom_left, uint32_t width, uint32_t height, COLORS color)
{
	v2 point1 = bottom_left;
	v2 point2 = V2(bottom_left.x , bottom_left.y + height);
	v2 point3 = V2(bottom_left.x + width, bottom_left.y + height);
	v2 point4 = V2(bottom_left.x + width, bottom_left.y);
	
	r_draw_line_br(px_buf, point1, point2, color);
	r_draw_line_br(px_buf, point2, point3, color);
	r_draw_line_br(px_buf, point3, point4, color);
	r_draw_line_br(px_buf, point4, point1, color);
}

internal void
r_draw_line_br(PIXEL_BUFFER* px_buf, v2 p0, v2 p1, COLORS color)
{
	int32_t dx = m_abs(p0.x - p1.x);
	int32_t sx = (p0.x < p1.x) ? 1 : -1;
	
	int32_t dy = -m_abs(p0.y - p1.y);
	int32_t sy = (p0.y < p1.y) ? 1 : -1;
	
	int32_t err = dx + dy;
	
	for(;;)
	{
		r_draw_point(px_buf, p0, color);
		if(p0.x == p1.x && p0.y == p1.y)
		{
			break;
		}
		int32_t e2 = 2 * err;
		
		if(e2 >= dy)
		{
			err += dy;
			p0.x += sx;
		}
		if(e2 <= dx)
		{
			err += dx;
			p0.y += sy;
		}
	}
}

internal void
r_cpy_bitmap_to_px_buff(PIXEL_BUFFER* px_buf, PIXEL_BUFFER* bitmap, uint32_t width, uint32_t height)
{
}