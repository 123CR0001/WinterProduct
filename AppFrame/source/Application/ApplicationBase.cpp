
#include "ApplicationBase.h"


ApplicationBase	*ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
	_pad = new XGamePad(DX_INPUT_PAD1);
}

ApplicationBase::~ApplicationBase() {

}

bool ApplicationBase::Initialize(HINSTANCE hInstance) {

	// DX���C�u�����̏�����
	if(AppWindowed()){
		ChangeWindowMode(true);							// �E�B���h�E���[�h�Ɏw�肷��
	}
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// �A�v������DXLib_Init()�O�ɂ�肽���������Ă�
	if(!BeforeDXLib_Init()) {
		return false;
	}

	if (DxLib_Init() == -1)
	{	// �G���[���N�����璼���ɏI��
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// �`����ʂ𗠉�ʂɃZ�b�g

	// �������߂��Ƃ���Ń`���c�L����������iZ�[�x������Ȃ��j�΍�
	SetZBufferBitDepth(32);

	// Effekseer������������B
// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return false;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// ����������
	srand((unsigned int)time(NULL));

	// ���[�h�T�[�o�̏�����
	_serverMode = NEW ModeServer();

	_isGameEnd = false;

	ResourceServer::Init();

	return true;
}

bool ApplicationBase::Terminate() {
	delete _pad;

	delete _serverMode;

	ResourceServer::Release();

	// Effekseer���I������B
	Effkseer_End();

	// DX���C�u�����J��
	DxLib_End();

	return true;
}


bool ApplicationBase::Input() {
	// �L�[�̓��́A�g���K���͂𓾂�
	int keyold = _gKey;
	_gKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	_gTrg = (_gKey ^ keyold) & _gKey;	// �L�[�̃g���K��񐶐��i�������u�Ԃ����������Ȃ��L�[���j

	_pad->Input();

	old_mouse_x = _mouse_x;
	old_mouse_y = _mouse_y;

	int old_mouse = _mouse_key;
	_mouse_key = GetMouseInput();
	_mouse_trg = (_mouse_key ^ old_mouse) & _mouse_key;
	_mouse_rel = (_mouse_key ^ old_mouse) & ~_mouse_key;

	GetMousePoint(&_mouse_x, &_mouse_y);


	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	return true;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}

