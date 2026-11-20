/*
 * @brief アイテムと魔法の効果
 * @author Sekino
 */
#pragma once
#ifndef _ITEMANDMAGICEFFECT_H_
#define _ITEMANDMAGICEFFECT_H_

#include <functional>
#include <vector>
#include "Vector2.h"

class CharacterData;

class ItemAndMagicEffect {
private:
	static std::function<void(int)> fixedMoveFunc;
	static std::function<Vector2(Vector2)> randomWarpFunc;
	static std::function<void()> moveEndFunc;
	static int usedItemAnyInfo;
public:
	static void EffectExecute(int effectID, int effectValue,int subEffectValue,CharacterData* _chara,std::vector<CharacterData*> _target = {});

	inline static void SetFixedMoveFunc(std::function<void(int)> _fixedMoveFunc) { fixedMoveFunc = _fixedMoveFunc; }

	inline static void SetRandomWarpFunc(std::function<Vector2(Vector2)> _randomWarpFunc) { randomWarpFunc = _randomWarpFunc; }

	inline static void SetMoveEndFunc(std::function<void()> _moveEndFunc) { moveEndFunc = _moveEndFunc; }

	inline static int GetUsedItemAnyInfo() { return usedItemAnyInfo; }
private:
	/*
	 * @brief ルーレットの数を増やす
	 */
	static void RouletteIncrease(int _effectValue, CharacterData* _chara);

	/*
	 * @brief 体力を回復する
	 */
	static void HPHeal(int _effectValue, CharacterData* _chara);

	/*
	 * @brief 指定マス数の移動
	 */
	static void FixedMove(int _effectValue, CharacterData* _chara);

	/*
	 * @brief ステータス倍化
	 */
	static void DoublingStatus(int _status,  CharacterData* _chara);

	/*
	 * @brief ギャンブル
	 */
	static void Gamble(int _prob, int _mag,CharacterData* _chara);

	/*
	 * @breif プレイヤーのスポーン地点に帰る
	 */
	static void GoHome(CharacterData* _chara);

	/*
	 * @brief プレイヤーをランダムな場所にワープさせる
	 */
	static void RandomWarp(CharacterData* _chara);
	
	/*
	 * @brief 指定キャラクターにダメージ
	 */
	static void Damage(int _effectValue,CharacterData* _chara,std::vector<CharacterData*> _target);

	/*
	 * @brief 位置入れ替え
	 */
	static void ChangePosition(CharacterData* _chara, CharacterData* _target);

	/*
	 * @brief 一方のほうに行く
	 */
	static void GoTargetPosition(CharacterData* _come, CharacterData* _go);

	/*
	 * @brief アイテムを一つ壊す
	 */
	static void BreakItem(CharacterData* _target);
};

#endif // !_ITEMEFFECT_H_