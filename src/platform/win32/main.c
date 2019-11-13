#include "logger.h"
#include "render.h"
#include "events.h"
#include "ogl_render.h"
#include "timer.h"
#include "gproj.h"


timer_hp_clk_t gproj_timer_hp_freq;

render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
render_finish_frame_fn_t render_finish_frame;



HWND hwnd = NULL;
HDC hdc = NULL;
static HINSTANCE hinstance;
static HINSTANCE hprevinstance;
static int showcmd;
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

static HANDLE log_handles[GPROJ_WIN32_LOG_HANDLE_NHANDLES];


static void platform_init(void)
{
	AttachConsole(ATTACH_PARENT_PROCESS);
	log_handles[GPROJ_WIN32_LOG_HANDLE_STDOUT] = GetStdHandle(STD_OUTPUT_HANDLE);
	log_handles[GPROJ_WIN32_LOG_HANDLE_STDERR] = GetStdHandle(STD_ERROR_HANDLE);

	log_dbg("INITIALIZING WIN32 PLATFORM");

	/* timer */
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	gproj_timer_hp_freq = freq.QuadPart;
}

static void platform_term(void)
{
	log_dbg("TERMINATING WIN32 PLATFORM");
	FreeConsole();
}

static void wm_create(HWND hWnd)
{
	hwnd = hWnd;
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

	HGLRC hgl_context = wglCreateContext(hdc);
	assert(hgl_context != NULL);

	if (wglMakeCurrent(hdc, hgl_context) == FALSE)
		INVALID_CODE_PATH;

	ogl_get_proc_addr("wglSwapIntervalEXT")(0);
	ogl_render_init();
}

static void wm_update_keys(WPARAM key, UINT msg)
{
	assert(msg == WM_KEYDOWN || msg == WM_KEYUP);

	input_button_t buttons = gproj_ev->input.buttons;
	for (int i = 0; i < STATIC_ARRAY_SIZE(winkeys); ++i) {
		if (winkeys[i] == key) {
			if (msg == WM_KEYUP) {
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

static void wm_size(void)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	const DWORD width = rect.right - rect.left;
	const DWORD height = rect.bottom - rect.top;
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1.0f, 1.0f);

	
	OGL_ASSERT_NO_ERROR();
}

static LRESULT window_proc_clbk(HWND hWnd,
                                UINT msg,
                                WPARAM wParam,
                                LPARAM lParam)
{
	if (gproj_ev == NULL)
		goto Lret;

	switch (msg) {
		case WM_CREATE:
			wm_create(hWnd);
			break;
		case WM_SIZE:
			wm_size();
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			wm_update_keys(wParam, msg);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			gproj_ev->flags |= EVENT_FLAG_QUIT;
			break;
	}

Lret:
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void dispatch_messages(void)
{
	static MSG msg;

	while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



void render_init(const char* const winname)
{
	log_dbg("INITIALIZING RENDER");

	struct events ev;
	gproj_ev = &ev;

	render_load_ts = ogl_render_load_ts;
	render_load_ss = ogl_render_load_ss;
	render_map = ogl_render_map;
	render_ss = ogl_render_ss;
	render_text = ogl_render_text;
	render_finish_frame = ogl_render_finish_frame;


	const LPCSTR app_name = winname;

	WNDCLASS wc;
	memset(&wc, 0, sizeof wc);

	wc.lpfnWndProc = window_proc_clbk;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
	wc.hInstance = hinstance;
	wc.lpszClassName = app_name;
    wc.lpszMenuName  = app_name; 
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
		hinstance,
		NULL
	);

	assert(hwnd != NULL);

	ShowWindow(hwnd, showcmd);
	UpdateWindow(hwnd);
	
	while (hdc == NULL)
		dispatch_messages();
	
	gproj_ev = NULL;
}

void render_term(void)
{
	log_dbg("TERMINATING RENDER");
}


void events_update(struct events* const in_gproj_ev)
{
	gproj_ev = in_gproj_ev;
	gproj_ev->flags = 0x00;
	dispatch_messages();
}

void gproj_win32_log_write(
	enum gproj_win32_log_handle handle,
	const char* fmt,
	...
)
{
	static char buffer[GPROJ_WIN32_LOG_BUFFER_SIZE];

	DWORD towrite, written;
	va_list valist;
	va_start(valist, fmt);
	towrite = vsnprintf(buffer, sizeof(buffer) - 1, fmt, valist);
	va_end(valist);
	
	buffer[towrite++] = '\n';
	
	WriteFile(log_handles[handle], buffer, towrite, &written, NULL);
}



int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	hinstance = hInstance;
	hprevinstance = hPrevInstance;
	showcmd = nShowCmd;

	platform_init();

	const int ret = gproj_main(0, NULL);

	platform_term();

	return ret;
}

