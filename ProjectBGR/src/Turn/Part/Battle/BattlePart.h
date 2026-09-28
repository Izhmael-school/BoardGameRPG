/*
 * @brief 戦闘パート
 * @author Sekino
 */

#pragma once
#ifndef _BATTLEPART_H_
#define _BATTLEPART_H_

enum BattleState {
	BattleState_AttackOrder,
	BattleState_SelectCommand,
	BattleState_Battle,
	BattleState_TurnChange,
	BattleState_Result,
	BattleState_LevelUp,
	BattleState_TurnEnd,
};

class CharacterData;
class GameObjectManager;
class GameObject;

#include <vector>
#include <functional>
#include "Command/Offense/OffenseCommand.h"
#include "Command/Defense/DefenseCommand.h"

constexpr int chara1 = 0;
constexpr int chara2 = 1;

class BattlePart {
private:
	// 状態
	BattleState currentState;
	// 対戦してるキャラクター
	std::vector<CharacterData*> players;
	// 先攻
	int attackPlayer;
	// 攻撃コマンド
	OffenseCommand offense;
	// 防御コマンド
	DefenseCommand defence;
	// 攻撃を食らったほう
	int damagePlayer;
	// 攻守交代したか
	bool isTurnChange;
	// 勝ったプレイヤー
	int winPlayer;
	// ターン終了処理
	std::function<void()> turnEnd;
	// 次のフェーズに行けるか
	bool canPhaseMove;
	// UI表示用
	int damage = 0;
	// カメラ
	GameObject* camera;
	// ステージ
	GameObject* stage;
	// カメラステージ取得用
	GameObjectManager* gameObjectManager;
	// プレイヤー１オブジェクト
	GameObject* player1Object;
	// プレイヤー２オブジェクト
	GameObject* player2Object;
	// 降参したか
	bool isSurrender = false;

public:
	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief 描画
	 */
	void Render();

	/*
	 * @brief 生成時に初期化
	 */
	void Init(std::function<void()> _turnEndFunc,GameObjectManager* _gameObjectManager);

	/*
	 * @brief バトル前初期化
	 */
	void BattleStart(CharacterData* _p1, CharacterData* _p2);


private:
	/*
	 * @brief 先攻決め
	 */
	void AttackOrder();

	/*
	 * @brief コマンド選択
	 */
	void SelectCommand();

	/*
	 * @brief バトル開始
	 */
	BattleState Battle();

	/*
	 * @brief 攻守交替
	 */
	void TurnChange();

	/*
	 * @brief リザルト
	 */
	BattleState Result();

	/*
	 * @brief バトル終了
	 */
	void BattleEnd();

	/*
	 * @brief コマンドごとの処理
	 */
	void ResolveAction(OffenseSelectCommand _oc, DefenseSelectCommand _dc);

	/*
	 * @brief 降参コマンド
	 */
	void Surrender();

	/*
	 * @brief ダメージ計算
	 */
	int CalcDamage(int _atk, int _def,bool _isGuard);

	/*
	 * @brief ダメージを与える
	 */
	void AddDamage(int _dmg, CharacterData* _target);

	/*
	 * @brief ボタンを押して次のフェーズに移行
	 */
	bool MovePhase(BattleState _nextState);
};

#endif