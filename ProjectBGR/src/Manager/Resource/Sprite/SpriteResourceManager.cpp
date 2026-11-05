#include "SpriteResourceManager.h"
#include "File/MyFile.h"
#include "Assert/MyAssert.h"
#include "Dxlib.h"
#include <cassert>

SpriteResourceManager::SpriteResourceManager() {
	LoadAllSprite();
}

SpriteResourceManager::~SpriteResourceManager() {
}

void SpriteResourceManager::LoadSprite(const std::string& _name, const std::string& _path) {
	auto itr = sprites.find(_name);

	// 重複チェック
	if (itr != sprites.end())
		return;

	// ハンドル取得
	int handle = LoadGraph(_path.c_str());

	// 代入
	sprites[_name] = handle;
}

void SpriteResourceManager::LoadAllSprite() {
	// ファイルの読み込み
	auto data = MyFile::GetFileData(SPRITE_FILEPATH, ".png");

	for (auto& d : data) {
		LoadSprite(d.fileName, d.filePath);
	}
}

int SpriteResourceManager::GetSprite(const std::string& _name) {
	auto itr = sprites.find(_name);

	if (itr == sprites.end()) {
		MyAssert::DebugAssert("No Sprite : " + _name);
		return -1;
	}
	
	return itr->second;
}

void SpriteResourceManager::Clear() {
	for (auto& sprite : sprites) {
		int handle = sprite.second;
		if (handle >= 0)
			DeleteGraph(handle);
	}

	sprites.clear();
}
