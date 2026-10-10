/* date = June 13th 2026 7:40 pm */

#ifndef MAIN_H
#define MAIN_H

#define internal static

internal uint32_t running = 1;

enum window_flags {
	WINDOW_FLAG_USE_VIEWPORT = 0x1,
	WINDOW_FLAG_ROTATING_TRANSFORM = 0x2,
	WINDOW_FLAG_USE_DAMAGE_BUFFER = 0x4,
};

typedef struct buffer buffer;
struct buffer{
	struct wl_buffer *buffer;
	uint32_t *shm_data;
	uint32_t busy;
};

typedef struct ctx ctx;
struct ctx{
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_seat *seat;
	struct wl_shm *shm;
	struct xdg_wm_base *wm_base;
	struct wl_callback *callback;
	struct wl_surface* surface;
	struct xdg_surface* xdg_surface;
	struct xdg_toplevel* xdg_toplevel;
	buffer buffers[2];
	buffer *prev_buffer;
	struct uint32_t *shm_data;
	enum window_flags flags;
	enum wl_output_transform transform;
	uint32_t width, height, border, scale;
	uint32_t formats;
	uint32_t wait_for_configure;
};

internal struct wl_display* create_display(ctx* app_ctx);
internal void create_window(ctx* app_ctx, int width, int height, enum wl_output_transform transform, int scale);
internal void init_game(ctx* app_ctx);
internal void redraw(void *data, struct wl_callback *callback, uint32_t time);
internal void xdg_surface_handle_configure(void *data, struct xdg_surface *surface, uint32_t serial);

internal void registry_handle_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version);
internal void registry_handle_global_remove(void *data,struct wl_registry *wl_registry, uint32_t name);
internal void shm_format(void* data, struct wl_shm *wl_shm, uint32_t format);
internal void xdg_wm_base_ping(void *data, struct xdg_wm_base *shell, uint32_t serial);
internal void seat_capabilities(void *data, struct wl_seat *wl_seat, uint capabilities);
internal void seat_name(void *data, struct wl_seat *wl_seat, const char *name);

internal const struct 
wl_registry_listener registry_listener = 
{
	.global        = registry_handle_global,
	.global_remove = registry_handle_global_remove
};

internal struct
wl_shm_listener shm_listener = {
	shm_format
};

internal const struct 
xdg_wm_base_listener wm_base_listener = {
	xdg_wm_base_ping,
};
internal const struct 
xdg_surface_listener xdg_surface_listener = {
	xdg_surface_handle_configure
};


internal const struct
wl_seat_listener seat_listener = {
	seat_capabilities,
	seat_name
};


internal void xdg_top_level_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states);
internal void xdg_toplevel_handle_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height);
internal void xdg_toplevel_handle_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities);
internal void xdg_toplevel_handle_close(void *data, struct xdg_toplevel *xdg_toplevel);

internal
struct xdg_toplevel_listener xdg_toplevel_listener = 
{
	xdg_top_level_handle_configure,
	xdg_toplevel_handle_close,
	xdg_toplevel_handle_configure_bounds,
	xdg_toplevel_handle_wm_capabilities
};

internal void
signal_int(int signum);

internal buffer*
window_next_buffer(ctx* app_ctx);

internal int 
os_create_anonymous_file(off_t size);

internal int
create_shm_buffer(ctx*, buffer* , int, int, uint32_t format);

internal void
buffer_release(void *data, struct wl_buffer *buffer);

internal const
struct wl_buffer_listener buffer_listener = 
{
	buffer_release
};

internal
struct wl_callback_listener frame_listener = {
	.done = redraw
};

#endif //MAIN_H
