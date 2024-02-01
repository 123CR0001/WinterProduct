
#include "appframe.h"


class ModeTitle : public ModeBase
{
	typedef ModeBase base;
	typedef ResourceServer res;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:


	class		UIServer* _ui;
	const int	MAX_OPTIONS = 2;
	int				_select;

}; 
