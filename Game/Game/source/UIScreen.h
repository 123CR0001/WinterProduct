#pragma once
#include<vector>

class UIScreen {
public:
	UIScreen();
	~UIScreen();

	bool Process();
	bool Draw();

	void AddUI(class UI* addUI);
	void DeleteUI(class UI* deleteUI);

	class UI* Get(const char* name);

private:

	std::vector<class UI*>_UIs;
	std::vector<class UI*>_addUIs;
	std::vector<class UI*>_deleteUIs;

};