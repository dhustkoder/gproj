#include "logger.h"
#include "events.h"
#include "render.h"
#include "events.h"
#include "ogl_render.h"


render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
render_finish_frame_fn_t render_finish_frame;


static HWND hwnd;
HDC hdc;
static HGLRC hglrc;
static MSG msg;
static BOOL win_destroy_req = false;
static struct events* gproj_ev = NULL;



static LRESULT window_proc_clbk(HWND hwnd,
                                UINT msg,
                                WPARAM wParam,
                                LPARAM lParam)
{
	if (gproj_ev == NULL)
		goto Lret;

	static PAINTSTRUCT ps;
	switch (msg) {
		case WM_PAINT:
			break;
		case WM_SIZE:
			break;
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			gproj_ev->flags |= EVENT_FLAG_QUIT;
			return WM_DESTROY;
	}
Lret:
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void render_init(const char* const winname)
{
	LOG_DEBUG("INITIALIZING RENDER");

	WNDCLASS wc;
	memset(&wc, 0, sizeof wc);

	wc.style = CS_OWNDC;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpfnWndProc = window_proc_clbk;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"GPROJ WINDOW";
	RegisterClass(&wc);


	hwnd = CreateWindow(
		wc.lpszClassName,
		winname,
		WS_OVERLAPPEDWINDOW,
		0, 0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
		NULL, NULL,
		wc.hInstance,
		NULL
	);	

	assert(hwnd != NULL);

	hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof pfd);

	pfd.nSize = sizeof pfd;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER|PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	const DWORD pf = ChoosePixelFormat(hdc, &pfd);
	assert(pf != 0);

	if (SetPixelFormat(hdc, pf, &pfd) == FALSE)
		INVALID_CODE_PATH;

	DescribePixelFormat(hdc, pf, sizeof pfd, &pfd);

	hglrc = wglCreateContext(hdc);
	assert(hglrc != NULL);

	if (wglMakeCurrent(hdc, hglrc) == FALSE)
		INVALID_CODE_PATH;

	ShowWindow(hwnd, SW_SHOW);

	render_load_ts = ogl_render_load_ts;
	render_load_ss = ogl_render_load_ss;
	render_map = ogl_render_map;
	render_ss = ogl_render_ss;
	render_text = ogl_render_text;
	render_finish_frame = ogl_render_finish_frame;

	ogl_render_init();
}

void render_term(void)
{
	LOG_DEBUG("TERMINATING RENDER");
}

void events_update(struct events* const in_gproj_ev)
{
	gproj_ev = in_gproj_ev;
	gproj_ev->flags = 0x00;

	while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);
}


