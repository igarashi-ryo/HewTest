#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirect3d.h"

static LPDIRECT3DDEVICE9 pDevice;

typedef struct GridVertex_tag
{
	D3DXVECTOR3 position;
	D3DCOLOR color;
}GridVertex;

static GridVertex g_grid_vertex[44];
#define FVF_GRID (D3DFVF_XYZ | D3DFVF_DIFFUSE)

void Grid_Init(void)
{
	//グリッド線のカラー初期化
	for (int i = 0; i <= 44; i++)
	{
		g_grid_vertex[i].color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}
	//グリッド線
	for (int i = 0; i < 22; i++)
	{
		int j = 43 - i;
		if (i % 2 == 0)
		{
			//iが偶数
			g_grid_vertex[i].position = D3DXVECTOR3(-5, 0, -5 + (i / 2));
			g_grid_vertex[j].position = D3DXVECTOR3(-5 + (i / 2), 0, -5);
		}
		else 
		{
			//iが奇数
			g_grid_vertex[i].position = D3DXVECTOR3(5, 0, -5 + (i / 2));
			g_grid_vertex[j].position = D3DXVECTOR3(-5 + (i / 2), 0, 5);
		}
	}
}

void Grid_Uninit(void)
{

}

void Grid_Update(void)
{
	
}

void Grid_Draw(void)
{
	pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);  //単位行列作成
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);  //デバイスにワールド変換を設定

	//描画処理
	pDevice->SetFVF(FVF_GRID);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 22, &g_grid_vertex, sizeof(GridVertex));
}
