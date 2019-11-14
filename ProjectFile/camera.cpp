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
	D3DXVECTOR3 eye(0.0f, 5.0f, -20.0f);  //�J�������W
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);  //����ꏊ
	at = HamPosition_Get();
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);  //���x�N�g���i�J�����̏㌈�߁j	
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up);  //�r���[�s��쐬
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);  //�f�o�C�X�Ƀr���[�ϊ��s���ݒ�

	D3DXMATRIX mtxProjection;
	//��p�w��
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);  //���W�A���p�ŉ�p�w��E�A�X�y�N�g��Enear�Efar
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}