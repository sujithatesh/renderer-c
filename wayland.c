//https://cgit.freedesktop.org/wayland/weston/tree/clients/simple-damage.c?__goaway_challenge=meta-refresh&__goaway_id=755e7b53026fdf079eb87e495bbeb394&__goaway_referer=https%3A%2F%2Fcgit.freedesktop.org%2Fwayland%2Fweston%2Ftree%2Fclients%3F__goaway_challenge%3Dmeta-refresh%26__goaway_id%3D1ad924cbf092bfbc4a18504c6e854017%26__goaway_referer%3Dhttps%253A%252F%252Fwww.google.com%252F

#include <stdio.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include "xdg-shell-client-protocol.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "wayland.h"
#include "bar.c"

internal void 
xdg_wm_base_ping(void *data, struct xdg_wm_base *shell, uint32_t serial)
{
	xdg_wm_base_pong(shell, serial);
}

internal void
registry_handle_global_remove(void *data,
															struct wl_registry *wl_registry,
															uint32_t name)
{
}

internal void
shm_format(void* data, struct wl_shm *wl_shm, uint32_t format)
{
	ctx *d = data;
	d->formats |= (1 << format);
}


internal void
registry_handle_global(void *data,
											 struct wl_registry *wl_registry,
											 uint32_t name,
											 const char *interface,
											 uint32_t version)
{
	ctx* app_ctx = (ctx *)data;
	if(strcmp(interface, "wl_compositor") == 0)
	{
		app_ctx->compositor = wl_registry_bind(app_ctx->registry, name, &wl_compositor_interface, version);
	}
	else if(strcmp(interface, "wl_seat") == 0)
	{
		app_ctx->seat = wl_registry_bind(app_ctx->registry, name, &wl_seat_interface, version);
		wl_seat_add_listener(app_ctx->seat, &seat_listener, app_ctx);
	}
	else if(strcmp(interface, "xdg_wm_base") == 0)
	{
		app_ctx->wm_base = wl_registry_bind(app_ctx->registry, name, &xdg_wm_base_interface, version);
		xdg_wm_base_add_listener(app_ctx->wm_base, &wm_base_listener, app_ctx);
	}
	else if(strcmp(interface, "wl_shm") == 0)
	{
		app_ctx->shm = wl_registry_bind(app_ctx->registry, name, &wl_shm_interface, version);
		wl_shm_add_listener(app_ctx->shm, &shm_listener, app_ctx);
	}
}

internal struct wl_display*
create_display(ctx* app_ctx)
{
	app_ctx->display = wl_display_connect(NULL);
	if (!app_ctx->display) 
	{
		fprintf(stderr, "Failed to connect to Wayland display.\n");
	}
	assert(app_ctx->display);
	
	app_ctx->registry = wl_display_get_registry(app_ctx->display);
	wl_registry_add_listener(app_ctx->registry, &registry_listener, app_ctx);
	
	app_ctx->formats = 0;
	
	// this round trip is to bind with all the interfaces in the wl_registry_add_listener
	wl_display_roundtrip(app_ctx->display);
	if (app_ctx->shm == NULL)
	{
		fprintf(stderr, "No wl_shm global\n");
	}
	
	// this is to get all the formats set by wl_shm_listener
	wl_display_roundtrip(app_ctx->display);
	return app_ctx->display;
}

internal void
init_game(ctx* app_ctx)
{}

internal void
create_window(ctx* app_ctx, int width, int height,
							enum wl_output_transform transform, int scale)
{
	app_ctx->callback  = NULL;
	app_ctx->width     = width;
	app_ctx->height    = height;
	app_ctx->border    = 10;
	app_ctx->transform = transform;
	app_ctx->scale     = scale;
	
	// app loop goes here ig??
	init_game(app_ctx);
	
	app_ctx->surface = wl_compositor_create_surface(app_ctx->compositor);
	app_ctx->xdg_surface = xdg_wm_base_get_xdg_surface(app_ctx->wm_base, app_ctx->surface); 
	assert(app_ctx->xdg_surface);
	
	xdg_surface_add_listener(app_ctx->xdg_surface, &xdg_surface_listener, app_ctx);
	app_ctx->xdg_toplevel =
		xdg_surface_get_toplevel(app_ctx->xdg_surface);
	assert(app_ctx->xdg_toplevel);
	
	xdg_toplevel_add_listener(app_ctx->xdg_toplevel, &xdg_toplevel_listener, app_ctx);
	xdg_toplevel_set_title(app_ctx->xdg_toplevel, "simple-damage");
	xdg_toplevel_set_app_id(app_ctx->xdg_toplevel,
													"org.freedesktop.weston.simple-damage");
	app_ctx->wait_for_configure = 1;
	wl_surface_commit(app_ctx->surface);
	
	// Something about Initialise damage to full surface, so the padding gets painted
	if(app_ctx->flags & WINDOW_FLAG_USE_DAMAGE_BUFFER)
	{
		wl_surface_damage_buffer(app_ctx->surface, 0, 0, INT32_MAX, INT32_MAX);
	}
	else {
		wl_surface_damage(app_ctx->surface, 0, 0, INT32_MAX, INT32_MAX);
	}
}

internal void
xdg_surface_handle_configure(void *data, struct xdg_surface *surface, uint32_t serial)
{
	struct ctx *app_ctx = data;
	xdg_surface_ack_configure(surface, serial);
	if(app_ctx->wait_for_configure) 
	{
		redraw(app_ctx, NULL, 0);
	}
}

internal void
xdg_toplevel_handle_close(void *data, struct xdg_toplevel *xdg_toplevel)
{
	running = 0;
}


internal void
xdg_toplevel_handle_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height)
{
}

internal void
xdg_top_level_handle_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states)
{
};


internal void
xdg_toplevel_handle_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities)
{
}

internal void
buffer_release(void *data, struct wl_buffer *buffer)
{
	struct buffer *my_buffer = data;
	my_buffer->busy = 0;
}

internal buffer*
window_next_buffer(ctx* app_ctx)
{
	buffer *buffer;
	int ret = 0, bwidth, bheight;
	
	if(!app_ctx->buffers[0].busy)
	{
		buffer = &app_ctx->buffers[0];
	}
	else if(!app_ctx->buffers[1].busy)
	{
		buffer = &app_ctx->buffers[1];
	}
	else
	{
		return NULL;
	}
	// NOTE(sujith): not handling transforms
	bwidth = app_ctx->width * app_ctx->scale;
	bheight = app_ctx->height * app_ctx->scale;
	
	if(!buffer->buffer)
	{
		ret = create_shm_buffer(app_ctx, buffer, bwidth, bheight, WL_SHM_FORMAT_ARGB8888);
	}
	return buffer;
}


internal void 
seat_capabilities(void *data, struct wl_seat *wl_seat, uint capabilities)
{
	
}

internal void
seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
	
}

internal int
os_create_anonymous_file(off_t size)
{
	const char *path = getenv("XDG_RUNTIME_DIR");
	if(!path)
	{
		path = "/tmp";
	}
	char template[256];
	snprintf(template, sizeof(template), "%s/wayland-shm-XXXXXX", path);
	int fd = mkstemp(template);
	if(fd<0) return -1;
	unlink(template);
	if(ftruncate(fd,size)<0)
	{
		close(fd);
		return -1;
	}
	return fd;
}

internal int
create_shm_buffer(ctx* app_ctx, buffer *buffer, int width, int height, uint32_t format)
{
	struct wl_shm_pool *pool;
	int fd, size, pitch;
	void *data;
	
	pitch = width * 4;
	size = pitch * height;
	
	fd = os_create_anonymous_file(size);
	
	data = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(data == MAP_FAILED)
	{
		fprintf(stderr, "mmap failed: %s\n", strerror(errno));
		close(fd);
		return -1;
	}
	pool = wl_shm_create_pool(app_ctx->shm, fd, size);
	buffer->buffer = wl_shm_pool_create_buffer(pool, 0, width, height, pitch, format);
	
	wl_buffer_add_listener(buffer->buffer, &buffer_listener, buffer);
	wl_shm_pool_destroy(pool);
	close(fd);
	buffer->shm_data = data;
	
	return 0;
}

internal void
redraw(void *data, struct wl_callback *callback, uint32_t time)
{
	ctx* app_ctx = data;
	buffer *buffer;
	uint32_t off_x = 0, off_y = 0;
	uint32_t bwidth, bheight, bpitch, bradius;
	float bx, by;
	
	buffer = window_next_buffer(app_ctx);
	app_ctx->wait_for_configure = 0;
	
	uint32_t *pixels = (uint32_t *) buffer->shm_data;
	
	
	PIXEL_BUFFER *px_ctx = malloc(sizeof(PIXEL_BUFFER)) ;
	px_ctx->width = app_ctx->width;
	px_ctx->height = app_ctx->height;
	px_ctx->pixels = pixels;
	populate_pixel_buffer(px_ctx);
	buffer->busy = 1;
	wl_surface_attach(app_ctx->surface,buffer->buffer, 0, 0);
	wl_surface_damage(app_ctx->surface, 0, 0, app_ctx->width, app_ctx->height);
	wl_surface_commit(app_ctx->surface);
}

internal void
signal_int(int signum)
{
	running = 0;
}

int main(void)
{
	struct sigaction sigint;
	int width = 1200, height = 900, scale = 1, ret = 0;
	ctx* app_ctx = (ctx*)malloc(sizeof(ctx));
	app_ctx->display = create_display(app_ctx);
	create_window(app_ctx, width, height, WL_OUTPUT_TRANSFORM_NORMAL, 1);
	sigint.sa_handler = signal_int;
	sigemptyset(&sigint.sa_mask);
	sigint.sa_flags = SA_RESETHAND;
	if(!app_ctx->wait_for_configure)
	{
		redraw(app_ctx, NULL, 0);
	}
	while(running && ret != -1)
	{
		ret = wl_display_dispatch(app_ctx->display);
	}
	return 0;
}
