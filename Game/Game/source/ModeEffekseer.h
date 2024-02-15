
#include "appframe.h"


#include <string>
#include <vector>
#include<unordered_map>


constexpr auto MODE_EFFEKSEER_NAME = "effectsample";

// ÉÇÅ[Éh
class ModeEffekseer : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	virtual int Play(std::string name,const Vector3D& pos,const Vector3D& angle);

protected:

	std::unordered_map<std::string,int>_effectHandle;

}; 
