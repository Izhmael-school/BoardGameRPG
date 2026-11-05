#include "ModelResourceManager.h"
#include <DxLib.h>
#include "File/MyFile.h"
#include "Assert/MyAssert.h"
#include <cassert>

ModelResourceManager::ModelResourceManager() {
	LoadAllModel();
}

ModelResourceManager::~ModelResourceManager() {
}

void ModelResourceManager::LoadModel(const std::string& _name, const std::string& _path){
	auto it = models.find(_name);

	// 重複チェック
	if (it != models.end())
		return;

	// ハンドル取得
	int handle = MV1LoadModel(_path.c_str());

	// 代入
	models[_name] = handle;

	return;
}

int ModelResourceManager::GetDupModelHandle(const std::string& _name) {
	auto itr = models.find(_name);

	if (itr == models.end()) {
		
		MyAssert::DebugAssert("Nothing Model : " + _name);

		return -1;
	}
	return MV1DuplicateModel(itr->second);
}

void ModelResourceManager::LoadAllModel() {
	// ファイルの読み込み
	auto data = MyFile::GetFileData(MODEL_FILEPATH, ".mv1");

	for (auto& d : data) {
		LoadModel(d.fileName, d.filePath);
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