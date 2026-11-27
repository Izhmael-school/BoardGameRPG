/*
 * @brief カメラの管理
 * @author Sekino
 */
#pragma once
#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

#include <vector>
#include <string>

class NormalCamera;

class CameraManager{
private: 
	std::vector<NormalCamera*> cameras;

public:
	CameraManager();
	~CameraManager() = default;

	void AddCamera(NormalCamera* _camera);

	void SetActive(std::string _objectName);

	template<class T>
	T* Get(std::string _name);
};

#endif // !_CAMERAMANAGER_H_

#include <typeindex>

template<class T>
inline T* CameraManager::Get(std::string _name) {
	if (!std::is_base_of<NormalCamera, T>::value) return nullptr;

	for (auto cam : cameras) {

		if (cam->GetName() != _name) continue;

		auto c = static_cast<T*>(cam);

		if (!c) return nullptr;

		return c;
	}

	return nullptr;
}
