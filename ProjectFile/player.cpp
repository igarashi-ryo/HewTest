#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirect3d.h"
#include "input.h"

static LPDIRECT3DDEVICE9 g_pDevice;
static float g_angle = 0.0f;
static int g_count = 0;

typedef struct PlayerVertex_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
}PlayerVertex;

//プレイヤー用キューブ設定(白)
static const PlayerVertex g_cube_vertex[] = {
	//正面
	{ D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	//右面
	{ D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	//左面
	{ D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	//後面
	{ D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255) },
	//上面
	{ D3DXVECTOR3(-0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,-.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,0.5f,-0.5f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	//下面
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255) }
};
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL)


void Player_Init(void)
{
	g_pDevice = MyDirect3D_GetDevice();
}

void Player_Uninit(void)
{

}

void Player_Update(void)
{
	if (Keyboard_IsPress(DIK_SPACE))
	{
		g_angle -= 0.1f;
	}
}

void Player_Draw(void)
{
	D3DXMATRIX mtxNull, mtxWorld, mtxRotation, mtxTranslation;

	D3DXMatrixIdentity(&mtxNull);  //単位行列作成

	//描画処理
	g_pDevice->SetFVF(FVF_CUBE);
	g_pDevice->SetTexture(0, NULL);

	D3DXMatrixRotationY(&mtxRotation, g_angle);  //angleラジアンY軸回転する行列の作成
	D3DXMatrixTranslation(&mtxTranslation, 0.0f, 0.5f, 0.0f);
	mtxWorld = mtxRotation * mtxTranslation;
	g_pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);  //デバイスにワールド変換を設定
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &g_cube_vertex, sizeof(PlayerVertex));

	D3DXMatrixTranslation(&mtxTranslation, 0.0f, 1.5f, 0.0f);
	mtxWorld = mtxRotation * mtxTranslation;
	g_pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &g_cube_vertex, sizeof(PlayerVertex));
}
