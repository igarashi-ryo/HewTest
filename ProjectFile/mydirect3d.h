#ifndef _MYDIRECT3D_H_
#define _MYDIRECT3D_H_

#include<Windows.h>

//Update,Draw�͂����ł͎g��Ȃ�
bool MyDirect3D_Init(HWND hWnd);
void MyDirect3D_Uninit(void);
//Device���擾�p(�Q�b�^�[)
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif