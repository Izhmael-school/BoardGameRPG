#include "CameraManager.h"
#include "GameObject/Camera/NormalCamera.h"
#include "Component/Camera/Camera.h"

CameraManager::CameraManager() 
	:cameras()
{
}

void CameraManager::AddCamera(NormalCamera* _camera) {
	auto itr = std::find(cameras.begin(), cameras.end(), _camera);
	// 重複排除
	if (itr == cameras.end()) return;
	cameras.emplace_back(_camera);
}

void CameraManager::SetActive(std::string _objectName) {
	for (auto cam : cameras) {

		bool isActive = cam->GetName() == _objectName;
		
		cam->SetActive(isActive);
		auto comp = cam->GetComponent<class Camera>();

		if (comp)
			comp->SetActive(isActive);
	}
}
