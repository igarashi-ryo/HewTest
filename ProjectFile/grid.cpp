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
	//�O���b�h���̃J���[������
	for (int i = 0; i <= 44; i++)
	{
		g_grid_vertex[i].color = D3DCOLOR_RGBA(0, 255, 0, 255);
	}
	//�O���b�h��
	for (int i = 0; i < 22; i++)
	{
		int j = 43 - i;
		if (i % 2 == 0)
		{
			//i������
			g_grid_vertex[i].position = D3DXVECTOR3(-5, 0, -5 + (i / 2));
			g_grid_vertex[j].position = D3DXVECTOR3(-5 + (i / 2), 0, -5);
		}
		else 
		{
			//i���
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

	D3DXMatrixIdentity(&mtxWorld);  //�P�ʍs��쐬
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);  //�f�o�C�X�Ƀ��[���h�ϊ���ݒ�

	//�`�揈��
	pDevice->SetFVF(FVF_GRID);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 22, &g_grid_vertex, sizeof(GridVertex));
}
