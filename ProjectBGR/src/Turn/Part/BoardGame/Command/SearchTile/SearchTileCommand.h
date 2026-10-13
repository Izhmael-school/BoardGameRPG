/*
 * @brief マスの効果やマスにいるキャラクターの情報を確認する
 * @author Sekino
 */

#pragma once
#ifndef _SEARCHTILECOMMAND_H_
#define _SEARCHTILECOMMAND_H_

class MapManager;
class CharacterDataManager;
class GameObject;

#include "Vector2.h"

class SearchTileCommand {
private:
	int currentX, currentY;
	int mapMaxX, mapMaxY;
	MapManager* map;
	CharacterDataManager* character;

	GameObject* mapArrow;

public:
	void Init(MapManager* _map,CharacterDataManager* _character,GameObject* _mapArrow);

	/*
	 * @brief 情報を確認するマスを決める
	 */
	void SearchTile();

	void Render();

	Vector2 GetCurrentPosition() const { return Vector2(currentX, currentY); }
private:
};

#endif // !_SEARCHTILECOMMAND_H_