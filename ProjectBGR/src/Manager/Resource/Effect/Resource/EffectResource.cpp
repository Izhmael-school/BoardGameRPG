#include "EffectResource.h"
//#include "EffekseerForDXLib.h"
#include <cassert>

EffectResource::EffectResource(const std::string& _name, const std::string& _path, float _magnification)
	:ResourceBase(_name, _path)
	,magnification(_magnification)
{}

EffectResource::~EffectResource() {
	if (IsLoaded());
		//DeleteEffekseerEffect(loadHandle);
}

bool EffectResource::Load() {
	// 二重ロードを防ぐ
	if (IsLoaded()) return false;

	// 読み込み
	//loadHandle = LoadEffekseerEffect(path.c_str(), magnification);

	if (!IsLoaded()) {
		// 失敗
#if _DEBUG
		assert(false && "Failed Load Effect");
#endif // _DEBUG
		return false;
	}

	return true;
}
