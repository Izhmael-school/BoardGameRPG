#include "AudioResource.h"
#include "DxLib.h"
#include <cassert>

AudioResource::AudioResource(const std::string& _name, const std::string& _path, bool _is3D) 
	:ResourceBase(_name, _path)
	, is3D(_is3D)
{}

AudioResource::~AudioResource() {
	if (IsLoaded())
		DeleteSoundMem(loadHandle);
}

bool AudioResource::Load() {
	if (IsLoaded()) return false;

	// 3D縺ｫ縺吶ｋ縺・
	SetCreate3DSoundFlag(static_cast<int>(is3D));

	// 隱ｭ縺ｿ霎ｼ縺ｿ
	loadHandle = LoadSoundMem(path.c_str());

	if (!IsLoaded()) {
		// 螟ｱ謨・
#if _DEBUG
		assert(false && "Failed Load Audio");
#endif // _DEBUG
		return false;
	}

	return true;
}
