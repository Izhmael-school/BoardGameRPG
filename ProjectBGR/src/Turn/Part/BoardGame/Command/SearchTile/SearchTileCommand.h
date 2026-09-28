/*
 * @brief マスの効果やマスにいるキャラクターの情報を確認する
 * @author Sekino
 */

#pragma once
#ifndef _SEARCHTILECOMMAND_H_
#define _SEARCHTILECOMMAND_H_

class MapManager;
class CharacterDataManager;

class SearchTileCommand {
private:
	int currentX, currentY;
	int mapMaxX, mapMaxY;
	MapManager* map;
	CharacterDataManager* character;

public:
	void Init(MapManager* _map,CharacterDataManager* _character);

	/*
	 * @brief 情報を確認するマスを決める
	 */
	void SearchTile();

	void Render();
private:
};

#endif // !_SEARCHTILECOMMAND_H_