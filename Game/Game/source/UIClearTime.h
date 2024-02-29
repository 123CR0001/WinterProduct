#pragma once

#include "UIBase.h"

class UIClearTime : public UIBase {
	typedef ResourceServer res;
public:
	UIClearTime();
	~UIClearTime();

	void Render() override;

protected:
#define TIME_NUM 10
	std::map<std::string, std::vector<int>> _mCg;
	std::map<std::string, int> _mDigit;
	int _clearTime;
};

