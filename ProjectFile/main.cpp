/*======================================================

HEW [main.cpp]
                                Auther : Kaito Tatsukawa
								Date   : 2019/09/24
--------------------------------------------------------

======================================================*/
#include <Windows.h>
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
//���w�����g�����ɕK�v�����A���̒��ɂ������Ă���
#include <math.h>
#include "common.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "system_timer.h"
#include"camera.h"
#include "player.h"
#include "hammer.h"
#include "grid.h"
#include "light.h"
#include "input.h"




/*------------------------------------------------------
�萔��`
------------------------------------------------------*/
#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "�RD�`��"
#define WINDOW_STYLE WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)
#define FPS_MEASUREMENT_TIME (1.0)


/*------------------------------------------------------
�v���g�^�C�v�錾
------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//.cpp�ɏ����v���g�^�C�v�錾�͑O��static��t����E�O���[�o���֐��Ɋւ��Ă�����
//exturn��goto�͎g���ȁI�I�I
static void Init(HWND hWnd);
static void Uninit(void);
static void Update(void);
static void Draw(void);
static int g_FrameCount = 0;  //�t���[���J�E���^
static int g_FPSBaseFrameCount = 0;  //FPS�v���p�t���[���J�E���^
static double g_FPSBaseTime = 0.0;  //FPS�v���p����
static float g_FPS = 0.0f;  //FPS
static double g_StaticFrameTime = 0.0;  //�t���[���Œ�p(IMMEDIATE�ł�����FPS�ɂȂ�悤��)
LPDIRECT3DDEVICE9 g_pDevice = NULL;
/*------------------------------------------------------
���C��
------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;   //�E�B���h�E�v���V�[�W���i�R�[���o�b�N�֐��̓o�^�j�V�X�e�����烁�b�Z�[�W���󂯎��֐��̓o�^
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  //�}�E�X�J�[�\�����E�B���h�E�p�[�c�ɗ������ɃJ�[�\���̌����ڂ��ǂ����邩
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);  //�w�i�̐F�@���͍�
	
	RegisterClass(&wc);

	RECT window_rect{ 0,0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT };

	//������RECT�\����,�E�B���h�E�T�C�Y,���j���[�����邩�ǂ�����bool�^
	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	//CreateWindow�F�E�B���h�E�̍쐬�i�o�^����Ă�����̂��쐬�ł���j
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		//WS�̓r�b�g���Z�Œǉ��E�폜���邱�Ƃ��o����FWS_OVERLAPPEDWINDOW | WS_HSCROLL�̂悤��
		//�r�b�g���Z�For |  and &  xor ^
		WINDOW_STYLE,  //�E�B���h�E�̌`�iWS�F�E�B���h�E�X�^�C���j
		//�f�t�H���g�FCW_USEDEFAULT
		window_x,  //x
		window_y,  //y
		window_width,  //��
		window_height,  //����
		NULL,
NULL,
hInstance,
NULL
);

ShowWindow(hWnd, nCmdShow);  //�E�B���h�E�̕\��
UpdateWindow(hWnd);  //ShowWindow�̎��ɃE�B���h�E�̍X�V�����Ȃ��Ɠ����Ȃ��Ƃ�������i���ɂ���āj

//���b�Z�[�W���[�v�F���ł��郁�b�Z�[�W���󂯎��҂�����
//��GetMessage()���b�Z�[�W���󂯎��܂Ńv���O�������u���b�N����
//GetMessege�͖߂�l��BOOL�^ BOOL�^�F�Sbite Windoes.h���K�v(TRUE,FALSE) bool�^�F�Pbite C++�Ŏg����(true,false)
MSG msg = {};

//GetMessage��WM_QUIT�������Ƃ��̂ݖ߂�l���O�ɂȂ�
Init(hWnd);
//Input.h�̏�����
Keyboard_Initialize(hInstance, hWnd);
while (WM_QUIT != msg.message) {
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);  //�|��݂����Ȃ��
		DispatchMessage(&msg);
	}
	else {
		
		double time = SystemTimer_GetTime();
		//���Ԃ�����܂ŋx��(60�t���[���ŌŒ�)
		if (time - g_StaticFrameTime < 1.0 / 60.0)
		{
			Sleep(0);  //������������CPU�ɋ󂫂��ł��邩������Ȃ�
		}
		else
		{
			g_StaticFrameTime = time;
			//�Q�[������
			Update();
			Draw();
		}
	}
}
Uninit();

return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		//�L�[�{�[�h���͂�wParam�ɓ���
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		//WM_QUIT���Ăяo�������ʃE�B���h�E���I������
		PostQuitMessage(0);
		return 0;
	};
	//���ʂ̓��������Ăق������͂���������Ă����i�S�������Ȃ��Ɠ����Ȃ��Ƃ����̂�h���j
	//���I������������switch (uMsg){}�Ŏ��s���Ȃ��Ƃ����Ȃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//������
void Init(HWND hWnd) {
	MyDirect3D_Init(hWnd);
	DebugFont_Initialize();
	SystemTimer_Initialize();
	SystemTimer_Start();
	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0f;
	Grid_Init();
	Light();
	Player_Init();
	Hammer_Init();
}

//�I������
void Uninit(void) {
	Keyboard_Finalize();
	Grid_Uninit();
	Player_Uninit();
	Hammer_Uninit();
	MyDirect3D_Uninit();
}

//�X�V
void Update(void) {
	g_FrameCount++;
	Keyboard_Update();
	double time = SystemTimer_GetTime();  //���̎��Ԃ��Ƃ�
	//��b�Ɉ��v������
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME)
	{
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}
	Camera_Update();
	Grid_Update();
	Player_Update();
	Hammer_Update();
}

//�`��
void Draw(void) {
	g_pDevice = MyDirect3D_GetDevice();
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	
	Grid_Draw();
	Player_Draw();
	Hammer_Draw();

	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}
