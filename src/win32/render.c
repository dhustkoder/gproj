#include "logger.h"
#include "render.h"
#include "events.h"
#include "ogl_render.h"


render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
//render_start_frame_fn_t render_start_frame;
render_finish_frame_fn_t render_finish_frame;


static void dummy() 
{

}

void render_init(const char* const name,
                 const render_mode_t mode)
{
	LOG_DEBUG("INITIALIZING WINDOW");
	render_load_ts = dummy;
	render_load_ss = dummy;
	render_map = dummy;
	render_ss = dummy;
	render_text = dummy;
	//render_start_frame = dummy;
	render_finish_frame = dummy;
}

void render_term(void)
{
	LOG_DEBUG("TERMINATING WINDOW");
}

