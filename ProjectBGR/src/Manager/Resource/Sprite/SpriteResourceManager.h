/*
 * @brief ゲーム中に使用する画像のハンドル管理
 * @author Sekino
 */
#pragma once
#ifndef _SPRITERESOURCEMANAGER_H_
#define _SPRITERESOURDEMANAGER_H_

#include <unordered_map>
#include <string>

const char* const SPRITE_FILEPATH = "res/Sprite/";

class SpriteResourceManager {
private:
	std::unordered_map<std::string, int> sprites;

public:
	SpriteResourceManager();
	~SpriteResourceManager();

	void LoadSprite(const std::string& _name,const std::string& _path);

	void LoadAllSprite();

	int GetSprite(const std::string& _name);

	inline int GetLoadedSpriteNum() const { return sprites.size(); }

	void Clear();
};

#endif