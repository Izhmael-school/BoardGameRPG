#include "ModelResourceManager.h"
#include <DxLib.h>
#include "Definition/CommonModule/Json/MyJson.h"
#include "Definition/CommonModule/String/MyString.h"
#include <cassert>

void ModelResourceManager::LoadModel(const std::string& _name, const std::string& _path){
	// すでにロード済みなら再利用
	auto it = models.find(_name);

	if (it != models.end()){
#if _DEBUG
		assert(false && "Model Loaded");
#endif
		return;
	}

	// モデルロード
	int handle = MV1LoadModel(_path.c_str());

	// 保存
	models[_name] = handle;

	return;
}

int ModelResourceManager::GetDupModelHandle(const std::string& _name) {
	auto itr = models.find(_name);

	if (itr == models.end()) {
#if _DEBUG
		assert(false && "Nothing Model");
#endif
		return -1;
	}
	return MV1DuplicateModel(itr->second);
}

void ModelResourceManager::LoadModelFromExternalFile() {
	auto data = MyJson::LoadJsonFile(MODELDATA_FILEPATH);

	for (auto& d : data) {
		std::string name = d["name"];
		std::string fileName = d["path"];
		std::string path = MyString::MergeString(MODEL_FILEPATH, fileName);

		LoadModel(name, path);
	}
}

void ModelResourceManager::Clear(){

	for (auto& model : models){
		if (model.second >= 0){
			MV1DeleteModel(model.second);
		}
	}

	models.clear();
}