#pragma once
#include<vector>

class MyUIServer {
public:
	MyUIServer();
	~MyUIServer();

	bool Process();
	bool Draw();

	void AddUI(class UI* addUI);
	void DeleteUI(class UI* deleteUI);

private:

	std::vector<class UI*>_UIs;
	std::vector<class UI*>_addUIs;
	std::vector<class UI*>_deleteUIs;

};