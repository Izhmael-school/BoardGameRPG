#include "DxLib.h"
#include "Application.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// 繧｢繝励Μ縺ｮ繧ｹ繧ｿ繝ｼ繝・
	Application::GetInstance().Run();

	return 0;
}