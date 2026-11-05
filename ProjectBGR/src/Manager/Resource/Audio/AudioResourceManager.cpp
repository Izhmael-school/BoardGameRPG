#include "AudioResourceManager.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "File/MyFile.h"
#include "Assert/MyAssert.h"
#include <cassert>

AudioResourceManager::AudioResourceManager() {
	LoadAllAudio();
}

bool AudioResourceManager::LoadAudio(const std::string& _name, const std::string& _path, bool _is3D) {
	// 蜷悟錐縺ｮ逋ｻ骭ｲ遖∵ｭ｢
	if (!resources.empty())
		if (resources.contains(_name))
			return false;
	// 逕滓・
	auto resource = std::make_shared<AudioResource>(_name, _path, _is3D);
	// 螟ｱ謨励＠縺溘ｉ蟶ｰ繧・
	if (!resource->Load()) return false;
	// 謌仙粥縺励◆繧蛾・蛻励↓
	resources.emplace(_name.c_str(), resource);
	return true;
}

void AudioResourceManager::LoadAllAudio() {
	auto data = MyFile::GetFileData(AUDIO_FILEPATH, ".mp3");

	for (auto& d : data) {
		LoadAudio(d.fileName, d.filePath, false);
	}

	// 3dサウンドのものはJsonで別途指定
	auto is3dResource = MyJson::LoadJsonFile(AUDIODATA_FILEPATH);
	for (auto& n : is3dResource) {
		std::string name = n["name"];

		if (!resources[name]) continue;

		resources[name]->Set3D(true);
	}
}

AudioResourcePtr AudioResourceManager::GetResource(const std::string& _name) const {
	auto itr = resources.find(_name);

	if (itr == resources.end()) {
		MyAssert::DebugAssert("No Audio : " + _name);
		return nullptr;
	}
	return itr->second;
}
