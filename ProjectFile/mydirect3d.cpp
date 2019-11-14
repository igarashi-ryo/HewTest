
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"common.h"
#include"mydirect3d.h"

//プロトタイプ宣言
static LPDIRECT3D9 g_pD3D = NULL;
//Direct3Dポインタ：デバイスの取得に必要
static LPDIRECT3DDEVICE9 g_pDevice = NULL;

bool MyDirect3D_Init(HWND hWnd) 
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		//MessageBox();
	}
	D3DPRESENT_PARAMETERS d3dpp = {};
	//BackBuffer：裏画面のこと→裏に表示し終わってから表と入れ替えを繰り返す
	d3dpp.BackBufferWidth = SCREEN_WIDTH;  //バックバッファの幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;  //バックバッファの高さ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  //バックバッファの種類
	d3dpp.BackBufferCount = 1;  //バックバッファの数：基本１
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  //表裏の入れ替え(スワップ)方法の指定
	d3dpp.Windowed = TRUE;  //ウィンドウモードかフルスクリーンか：フルスクリーンの場合、ウィンドウスタイルをポップアップにした方がいい
							//デプスバッファ：深度情報
	d3dpp.EnableAutoDepthStencil = TRUE;  //デプスバッファやステンシルバッファを自動で設定するかどうか
										  //↑これがTRUEなら↓指定する
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;  //デプス16Bit（ステンシルバッファ使うときはここを変更する(D??S??)D：デプス S：ステンシル
												//リフレッシュレート：画面更新速度（ウィンドウモード関係なし）
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;  //リフレッシュレートの指定
																 //↓DEFAULT:タイマーの精度が低い　ONE:タイマーの精度がいいが起動が遅い　IMMEDIATE:ティアリング起こしてもいいけど、高速に動かせる(裏が書き終わったら表を待たずに入れ替える)ベンチマークとして使える
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;  //画面の更新タイミング
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;  //画面の更新タイミング

	HRESULT hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,  //アダプタの指定：どのモニタ使うの？ってやつ
		D3DDEVTYPE_HAL,  //ハードウェアで計算してねっていう指定
						 //対象のウィンドウハンドル
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,  //頂点の計算をハードウェアでやってねという指定
		&d3dpp,  //指示書の住所
		&g_pDevice  //メモ帳の住所
	);
	if (FAILED(hr)) {
		return false;
	}
	return true;
}
void MyDirect3D_Uninit(void)
{
	if (g_pDevice != NULL) {
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D != NULL) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//セッター
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}