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
//数学式を使う時に必要だが、↑の中にも入っている
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
定数定義
------------------------------------------------------*/
#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "３D描画"
#define WINDOW_STYLE WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)
#define FPS_MEASUREMENT_TIME (1.0)


/*------------------------------------------------------
プロトタイプ宣言
------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//.cppに書くプロトタイプ宣言は前にstaticを付ける・グローバル関数に関してもそう
//exturnとgotoは使うな！！！
static void Init(HWND hWnd);
static void Uninit(void);
static void Update(void);
static void Draw(void);
static int g_FrameCount = 0;  //フレームカウンタ
static int g_FPSBaseFrameCount = 0;  //FPS計測用フレームカウンタ
static double g_FPSBaseTime = 0.0;  //FPS計測用時間
static float g_FPS = 0.0f;  //FPS
static double g_StaticFrameTime = 0.0;  //フレーム固定用(IMMEDIATEでも一定のFPSになるように)
LPDIRECT3DDEVICE9 g_pDevice = NULL;
/*------------------------------------------------------
メイン
------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;   //ウィンドウプロシージャ（コールバック関数の登録）システムからメッセージを受け取る関数の登録
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  //マウスカーソルがウィンドウパーツに来た時にカーソルの見た目をどうするか
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);  //背景の色　今は黒
	
	RegisterClass(&wc);

	RECT window_rect{ 0,0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT };

	//引数はRECT構造体,ウィンドウサイズ,メニューがあるかどうかのbool型
	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	//CreateWindow：ウィンドウの作成（登録されているものを作成できる）
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		//WSはビット演算で追加・削除することが出来る：WS_OVERLAPPEDWINDOW | WS_HSCROLLのように
		//ビット演算：or |  and &  xor ^
		WINDOW_STYLE,  //ウィンドウの形（WS：ウィンドウスタイル）
		//デフォルト：CW_USEDEFAULT
		window_x,  //x
		window_y,  //y
		window_width,  //幅
		window_height,  //高さ
		NULL,
NULL,
hInstance,
NULL
);

ShowWindow(hWnd, nCmdShow);  //ウィンドウの表示
UpdateWindow(hWnd);  //ShowWindowの時にウィンドウの更新を入れないと動かないときがある（環境によって）

//メッセージループ：飛んでくるメッセージを受け取り待ちする
//→GetMessage()メッセージを受け取るまでプログラムをブロックする
//GetMessegeは戻り値はBOOL型 BOOL型：４bite Windoes.hが必要(TRUE,FALSE) bool型：１bite C++で使える(true,false)
MSG msg = {};

//GetMessageにWM_QUITが来たときのみ戻り値が０になる
Init(hWnd);
//Input.hの初期化
Keyboard_Initialize(hInstance, hWnd);
while (WM_QUIT != msg.message) {
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);  //翻訳みたいなやつ
		DispatchMessage(&msg);
	}
	else {
		
		double time = SystemTimer_GetTime();
		//時間が来るまで休む(60フレームで固定)
		if (time - g_StaticFrameTime < 1.0 / 60.0)
		{
			Sleep(0);  //もしかしたらCPUに空きができるかもしれない
		}
		else
		{
			g_StaticFrameTime = time;
			//ゲーム処理
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
		//キーボード入力がwParamに入る
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		//WM_QUITを呼び出す→結果ウィンドウが終了する
		PostQuitMessage(0);
		return 0;
	};
	//普通の動きをしてほしい物はこいつがやってくれる（全部書かないと動かないというのを防ぐ）
	//→終了処理だけはswitch (uMsg){}で実行しないといけない
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//初期化
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

//終了処理
void Uninit(void) {
	Keyboard_Finalize();
	Grid_Uninit();
	Player_Uninit();
	Hammer_Uninit();
	MyDirect3D_Uninit();
}

//更新
void Update(void) {
	g_FrameCount++;
	Keyboard_Update();
	double time = SystemTimer_GetTime();  //今の時間をとる
	//一秒に一回計測する
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

//描写
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
