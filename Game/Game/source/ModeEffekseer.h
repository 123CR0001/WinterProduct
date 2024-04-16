
#include "appframe.h"


#include <string>
#include <vector>
#include<unordered_map>


constexpr auto MODE_EFFEKSEER_NAME = "effectsample";

// ���[�h
class ModeEffekseer : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual int Play(std::string name,const Vector3& pos,const Vector3& angle);

protected:

	std::unordered_map<std::string,int>_effectHandle;			//�G�t�F�N�g�n���h��
	std::vector<int>_playingEffectHandles;						//�Đ����̃G�t�F�N�g�n���h��	
}; 
