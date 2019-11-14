#ifndef _MYDIRECT3D_H_
#define _MYDIRECT3D_H_

#include<Windows.h>

//Update,Drawはここでは使わない
bool MyDirect3D_Init(HWND hWnd);
void MyDirect3D_Uninit(void);
//Device情報取得用(ゲッター)
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif