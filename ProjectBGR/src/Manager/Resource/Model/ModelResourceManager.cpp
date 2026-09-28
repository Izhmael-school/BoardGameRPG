#include "ModelResourceManager.h"
#include <DxLib.h>
#include "Definition/CommonModule/Json/MyJson.h"
#include "Definition/CommonModule/String/MyString.h"
#include <cassert>

ModelResourceManager::ModelResourceManager() {
}

ModelResourceManager::~ModelResourceManager() {
}

void ModelResourceManager::LoadModel(const std::string& _name, const std::string& _path){
	// 縺吶〒縺ｫ繝ｭ繝ｼ繝画ｸ医∩縺ｪ繧牙・蛻ｩ逕ｨ
	auto it = models.find(_name);

	if (it != models.end()){
#if _DEBUG
		assert(false && "Model Loaded");
#endif
		return;
	}

	// 繝｢繝・Ν繝ｭ繝ｼ繝・
	int handle = MV1LoadModel(_path.c_str());

	// 菫晏ｭ・
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