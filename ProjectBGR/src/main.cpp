#include "DxLib.h"
#include "Application.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// アプリのスタート
	Application::GetInstance().Run();

	return 0;
}