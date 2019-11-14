
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"common.h"
#include"mydirect3d.h"

//�v���g�^�C�v�錾
static LPDIRECT3D9 g_pD3D = NULL;
//Direct3D�|�C���^�F�f�o�C�X�̎擾�ɕK�v
static LPDIRECT3DDEVICE9 g_pDevice = NULL;

bool MyDirect3D_Init(HWND hWnd) 
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		//MessageBox();
	}
	D3DPRESENT_PARAMETERS d3dpp = {};
	//BackBuffer�F����ʂ̂��Ɓ����ɕ\�����I����Ă���\�Ɠ���ւ����J��Ԃ�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;  //�o�b�N�o�b�t�@�̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;  //�o�b�N�o�b�t�@�̍���
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  //�o�b�N�o�b�t�@�̎��
	d3dpp.BackBufferCount = 1;  //�o�b�N�o�b�t�@�̐��F��{�P
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  //�\���̓���ւ�(�X���b�v)���@�̎w��
	d3dpp.Windowed = TRUE;  //�E�B���h�E���[�h���t���X�N���[�����F�t���X�N���[���̏ꍇ�A�E�B���h�E�X�^�C�����|�b�v�A�b�v�ɂ�����������
							//�f�v�X�o�b�t�@�F�[�x���
	d3dpp.EnableAutoDepthStencil = TRUE;  //�f�v�X�o�b�t�@��X�e���V���o�b�t�@�������Őݒ肷�邩�ǂ���
										  //�����ꂪTRUE�Ȃ火�w�肷��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;  //�f�v�X16Bit�i�X�e���V���o�b�t�@�g���Ƃ��͂�����ύX����(D??S??)D�F�f�v�X S�F�X�e���V��
												//���t���b�V�����[�g�F��ʍX�V���x�i�E�B���h�E���[�h�֌W�Ȃ��j
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;  //���t���b�V�����[�g�̎w��
																 //��DEFAULT:�^�C�}�[�̐��x���Ⴂ�@ONE:�^�C�}�[�̐��x���������N�����x���@IMMEDIATE:�e�B�A�����O�N�����Ă��������ǁA�����ɓ�������(���������I�������\��҂����ɓ���ւ���)�x���`�}�[�N�Ƃ��Ďg����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;  //��ʂ̍X�V�^�C�~���O
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;  //��ʂ̍X�V�^�C�~���O

	HRESULT hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,  //�A�_�v�^�̎w��F�ǂ̃��j�^�g���́H���Ă��
		D3DDEVTYPE_HAL,  //�n�[�h�E�F�A�Ōv�Z���Ă˂��Ă����w��
						 //�Ώۂ̃E�B���h�E�n���h��
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,  //���_�̌v�Z���n�[�h�E�F�A�ł���Ă˂Ƃ����w��
		&d3dpp,  //�w�����̏Z��
		&g_pDevice  //�������̏Z��
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

//�Z�b�^�[
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}