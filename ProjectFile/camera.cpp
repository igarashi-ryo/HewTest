#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirect3d.h"
#include "hammer.h"

static LPDIRECT3DDEVICE9 pDevice;

void Camera_Update(void)
{
	pDevice = MyDirect3D_GetDevice();



	D3DXMATRIX mtxView;
	D3DXVECTOR3 eye(0.0f, 5.0f, -20.0f);  //カメラ座標
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);  //見る場所
	at = HamPosition_Get();
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);  //情報ベクトル（カメラの上決め）	
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);  //ビュー行列作成
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);  //デバイスにビュー変換行列を設定

	D3DXMATRIX mtxProjection;
	//画角指定
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);  //ラジアン角で画角指定・アスペクト比・near・far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}