#include "EffectResourceManager.h"
#include "Resource/EffectResource.h"
#include "File/MyFile.h"
#include "Assert/MyAssert.h"

EffectResourceManager::EffectResourceManager() {
	LoadAllEffect();
}

bool EffectResourceManager::LoadEffect(const std::string& _name, const std::string& _path) {
	// 中身がなければ無視
	if (!resources.empty())
		if (resources.contains(_name)) {
			return false;
		}
	// 生成
	auto resource = std::make_shared<EffectResource>(_name, _path);
	// 再生できなければ生成失敗
	if (!resource->Load()) return false;
	// 配列に入れる
	resources.emplace(_name.c_str(), resource);
	return true;
}

void EffectResourceManager::LoadAllEffect() {
	// ファイルの読み込み
	auto data = MyFile::GetFileData(EFFECT_FILEPATH, "");

	for (auto& d : data) {
		LoadEffect(d.fileName, d.filePath);
	}
}

EffectResourcePtr EffectResourceManager::GetResource(const std::string& _name) const {
	auto itr = resources.find(_name);

	if (itr == resources.end()) {

		MyAssert::DebugAssert("Nothing Effect : " + _name);

		return nullptr;
	}

	return itr->second;
}