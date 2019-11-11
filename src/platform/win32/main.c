#include "logger.h"
#include "render.h"
#include "events.h"
#include "ogl_render.h"
#include "gproj.h"


render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
render_finish_frame_fn_t render_finish_frame;


HWND hwnd;
HDC hdc;

static HGLRC hglrc;
static MSG msg;
static struct events* gproj_ev = NULL;


static u8 winkeys[] = {
	0x57, // w
	0x53, // s
	0x41, // a
	0x44 // d
#ifdef GPROJ_DEBUG
	, 
	0x4B, // k
	0x4A, // j
	0x48, // h
	0x4C, // l
	0x4D, // m
	0x4E, // n
	0x42  // b
#endif
};

static input_button_t gprojkeys[] = {
	INPUT_BUTTON_UP,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT

#ifdef GPROJ_DEBUG
	,
	INPUT_BUTTON_CAM_UP,
	INPUT_BUTTON_CAM_DOWN,
	INPUT_BUTTON_CAM_LEFT,
	INPUT_BUTTON_CAM_RIGHT,
	INPUT_BUTTON_WORLD_SCALE_UP,
	INPUT_BUTTON_WORLD_SCALE_DOWN,
	INPUT_BUTTON_WORLD_FWD
#endif
};



static void update_keys(WPARAM key, UINT evtype)
{
	assert(evtype == WM_KEYDOWN || evtype == WM_KEYUP);

	input_button_t buttons = gproj_ev->input.buttons;
	for (int i = 0; i < STATIC_ARRAY_SIZE(winkeys); ++i) {
		if (winkeys[i] == key) {
			if (evtype == WM_KEYUP) {
				buttons &= ~gprojkeys[i];
			} else {
				buttons |= gprojkeys[i];
			}
		}
	}

	if (buttons != gproj_ev->input.buttons) {
		gproj_ev->input.buttons = buttons;
		gproj_ev->flags |= EVENT_FLAG_NEW_INPUT;
	}
}

static void window_size_update(void)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	const DWORD width = rect.right - rect.left;
	const DWORD height = rect.bottom - rect.top;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	OGL_ASSERT_NO_ERROR();
	glLoadIdentity();
	OGL_ASSERT_NO_ERROR();
	glOrtho(0, width, height, 0, -1.0f, 1.0f);
	OGL_ASSERT_NO_ERROR();
}


static LRESULT window_proc_clbk(HWND hWnd,
                                UINT evtype,
                                WPARAM wParam,
                                LPARAM lParam)
{
	if (gproj_ev == NULL)
		goto Lret;

	switch (evtype) {
		case WM_SIZE:
			window_size_update();
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			update_keys(wParam, evtype);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			gproj_ev->flags |= EVENT_FLAG_QUIT;
			return WM_DESTROY;
	}

Lret:
	return DefWindowProc(hWnd, evtype, wParam, lParam);
}



void render_init(const char* const winname)
{
	LOG_DEBUG("INITIALIZING RENDER");
	const HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc;
	memset(&wc, 0, sizeof wc);

	const LPSTR app_name = winname;	
	wc.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpfnWndProc = window_proc_clbk;
	wc.hInstance = hInstance;
	wc.lpszClassName = app_name;
	
	if (RegisterClass(&wc) == 0)
		INVALID_CODE_PATH;

	hwnd = CreateWindow(
		app_name,
		app_name,
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT,
		GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
		NULL, NULL,
		hInstance,
		NULL
	);

	assert(hwnd != NULL);



	hdc = GetDC(hwnd);
	assert(hdc != NULL);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof pfd);

	pfd.nSize = sizeof pfd;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER|PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = 32;


	const DWORD pf = ChoosePixelFormat(hdc, &pfd);
	assert(pf != 0);

	if (SetPixelFormat(hdc, pf, &pfd) == FALSE)
		INVALID_CODE_PATH;

	if (!DescribePixelFormat(hdc, pf, sizeof pfd, &pfd))
		INVALID_CODE_PATH;



	hglrc = wglCreateContext(hdc);
	assert(hglrc != NULL);

	if (wglMakeCurrent(hdc, hglrc) == FALSE)
		INVALID_CODE_PATH;

	OGL_GET_PROC_ADDR("wglSwapIntervalEXT")(1);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	SetForegroundWindow(hwnd);

	render_load_ts = ogl_render_load_ts;
	render_load_ss = ogl_render_load_ss;
	render_map = ogl_render_map;
	render_ss = ogl_render_ss;
	render_text = ogl_render_text;
	render_finish_frame = ogl_render_finish_frame;

	ogl_render_init();
	window_size_update();
}

void render_term(void)
{
	LOG_DEBUG("TERMINATING RENDER");
}

void events_update(struct events* const in_gproj_ev)
{
	gproj_ev = in_gproj_ev;
	gproj_ev->flags = 0x00;

	while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}




int main(void)
{
	return gproj_main(0, NULL);
}

