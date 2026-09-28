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
	// 莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ縺・
	if (IsLoaded()) return false;

	// 隱ｭ縺ｿ霎ｼ縺ｿ
	//loadHandle = LoadEffekseerEffect(path.c_str(), magnification);

	if (!IsLoaded()) {
		// 螟ｱ謨・
#if _DEBUG
		assert(false && "Failed Load Effect");
#endif // _DEBUG
		return false;
	}

	return true;
}
