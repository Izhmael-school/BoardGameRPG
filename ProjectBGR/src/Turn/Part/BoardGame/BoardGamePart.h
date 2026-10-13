/*
 * @brief 縺吶＃繧阪￥繝代・繝・
 * @author Sekino
 */
#pragma once
#ifndef _BOARDGAMEPART_H_
#define _BOARDGAMEPART_H_

#include "Command/Item/ItemCommand.h"
#include "Command/Move/MoveCommand.h"
#include "Command/SearchTile/SearchTileCommand.h"
#include "Command/Magic/FieldMagicCommand.h"
#include "Shop/ShopPart.h"
#include "Church/ChurchPart.h"
#include "Village/VillagePart.h"
#include "BoardGameElement.h"
#include <string>
#include <functional>

class CharacterData;
class CharacterDataManager;
class GameObject;
class GameObjectManager;
class BoardGameCanvas;
class UIManager;

class BoardGamePart {
private:
	// 迴ｾ蝨ｨ縺ｮ繝励Ξ繧､繝､繝ｼ縺ｮ繝・・繧ｿ
	PlayerData* currentPlayerData;

	// 驕ｸ縺ｹ繧九さ繝槭Φ繝峨・驟榊・
	std::vector<Command> commandList;
	// 驕ｸ繧薙〒縺・ｋ繧ｳ繝槭Φ繝・
	int selectCommand;
	// 繧ｿ繝ｼ繝ｳ縺ｮ迥ｶ諷・
	TurnState turnState;

	// 繧｢繧､繝・Β繧ｳ繝槭Φ繝・
	ItemCommand itemCommand;
	// 鬲疲ｳ輔さ繝槭Φ繝・
	FieldMagicCommand magicCommand;
	// 遘ｻ蜍輔さ繝槭Φ繝・
	MoveCommand moveCommand;
	// マスのデータを確認するコマンド
	SearchTileCommand searchTileCommand;

	MapManager* map;

	CharacterDataManager* character;

	// 繧ｿ繝ｼ繝ｳ縺ｮ邨ゆｺ・・逅・
	std::function<void()> turnEndFunc;
	// バトルに遷移するためのイベント
	std::function<void(CharacterData*, CharacterData*)> battleStartFunc;
	// ターン中のプレイヤーがバトルできる対象
	std::vector<CharacterData*> canBattlePlayer;
	// 誰とバトルするか
	int battleTargetIndex;
	// 最初のターンか
	bool isFirstTurn = false;
	// ターンのカウント
	int turnCount = 0;
	// ステージのオブジェクト
	GameObject* stage;
	// ステージ取得用
	GameObjectManager* gameObjectManager;
	// カメラ
	GameObject* camera;
	// ターン中のプレイヤー
	GameObject* currentTurnPlayerObject;
	// キャンバス
	BoardGameCanvas* canvas;
	// ショップ
	ShopPart shopPart;
	// 教会
	ChurchPart churchPart;
	// 村
	VillagePart villagePart;

public:
	/*
	 * @brief 生成時に初期化
	 */
	void Init(UIManager* _uiManager,MapManager* _map,CharacterDataManager* _characterDataManager,std::function<void()> _turnEndFunc,std::function<void(CharacterData*, CharacterData*)> _battleStartFunc, GameObjectManager* _gameObjectManager,BoardGameCanvas* _canvas);

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief 描画
	 */
	void Render();

	/*
	 * @brief ターンプレイヤーを取得
	 */
	void SetCurrentPlayerData(PlayerData* _currentPlayerData);

	/*
	 * @brief ターン開始時処理
	 */
	void Setup();

	/*
	 * @brief 現在ターン中のプレイヤーの現在のタイルの列挙体を返す
	 */
	MapTileType GetCurrentPlayerMapTileType();

	/*
	 * @brief 外部から状態を変更する
	 */
	void SetTurnState(TurnState _state) { turnState = _state; }
private:

	/*
	 * @brief コマンド選択
	 */
	void CommandSelect();

	/*
	 * @brief 移動処理
	 */
	void MoveCommand();

	/*
	 * @brief アイテムコマンド
	 */
	void ItemCommand();

	/*
	 * @brief 魔法コマンド
	 */
	void MagicCommand();
	
	/*
	 * @brief バトルする対象を選ぶ(一人ならそのままバトルに移行する)
	 */
	void BattleCommand();

	/*
	 * @brief マスの情報を確認する
	 */
	void SearchTileCommand();

	/*
	 * @brief ターン終了処理
	 */
	void TurnEnd();

	/*
	 * @brief マスの効果
	 */
	void TileEffectExecute();

	/*
	 * @brief バトルに入るか確認しては入れるなら入る
	 */
	void InBattle();

	/*
	 * @brief 選べるコマンドを探す
	 */
	void PickUpCommand();

	/*
	 * @brief ショップに入る
	 */
	void ShopIn();

	/*
	 * @brief ショップから出る
	 */
	void ShopOut();
};

#endif