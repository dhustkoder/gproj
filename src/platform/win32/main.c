#include "logger.h"
#include "render.h"
#include "input.h"
#include "ogl_render.h"
#include "timer.h"
#include "game.h"


timer_hp_clk_t gproj_timer_hp_freq;

render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
render_finish_frame_fn_t render_finish_frame;


static HDC hdc = NULL;
static HWND hwnd = NULL;
static HINSTANCE hinstance;
static HINSTANCE hprevinstance;
static int showcmd;
static input_t ginput;
static b32 close_app_request = false;

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

static input_t gprojkeys[] = {
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

	input_t buttons = NEW_INPUT(ginput);

	for (int i = 0; i < STATIC_ARRAY_SIZE(winkeys); ++i) {
		if (winkeys[i] == key) {
			if (msg == WM_KEYUP) {
				buttons &= ~gprojkeys[i];
			} else {
				buttons |= gprojkeys[i];
			}
		}
	}

	INPUT_SET_NEW_VALUE(ginput, buttons);
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

static LRESULT window_proc_clbk(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (msg) {

		case WM_CREATE:
			wm_create(hWnd);
			break;
		case WM_SIZE:
			wm_size();
			break;

		case WM_KEYDOWN: case WM_KEYUP:
			wm_update_keys(wParam, msg);
			break;

		case WM_DESTROY:
			close_app_request = true;
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);

	}

	return TRUE;
}

static b32 update_events(void)
{
	static MSG msg;

	INPUT_UPDATE_OLD_VALUE(ginput);

	while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !close_app_request;
}

static void finish_frame_opengl(void)
{
	ogl_render_finish_frame();
	if (SwapBuffers(hdc) == FALSE)
		INVALID_CODE_PATH;
}


void render_init(const char* const winname)
{
	log_dbg("INITIALIZING RENDER");

	render_load_ts = ogl_render_load_ts;
	render_load_ss = ogl_render_load_ss;
	render_map = ogl_render_map;
	render_ss = ogl_render_ss;
	render_text = ogl_render_text;
	render_finish_frame = finish_frame_opengl;


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
	
	while (hdc == NULL) {
		if (update_events() == FALSE)
			break;
	}
}

void render_term(void)
{
	log_dbg("TERMINATING RENDER");
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
	buffer[towrite++] = '\n';
	
	WriteFile(log_handles[handle], buffer, towrite, &written, NULL);

	va_end(valist);
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
	game_init();

	timer_clk_t clk = timer_now();
	timer_clk_t lastclk = clk;

	while (update_events()) {
		const timer_clk_t now = timer_now();
		const float dt = (now - lastclk) / 1000.f;

		game_step(ginput, now, dt);
	
		lastclk = now;
	};

	game_term();
	platform_term();

	return EXIT_SUCCESS;
}

