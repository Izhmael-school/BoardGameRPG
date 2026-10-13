/*
 * @brief ボードゲーム内のショップマス
 * @author Sekino
 */
#pragma once
#ifndef _SHOPPART_H_
#define _SHOPPART_H_

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "ShopElement.h"

class ElementDataManager;
class PlayerData;
class UICanvasBase;
class UIManager;

class ShopPart {
private:
	ElementDataManager* elementDataManager;

	std::vector<BuyElement> buyArray;

	std::vector<SellElement> canSellArray;

	ShopState shopState;

	int modeSelection;

	int elementSelection;

	PlayerData* currentTurnPlayer;

	std::unique_ptr<UICanvasBase> shopCanvas;

	UIManager* uiManager;

	std::function<void()> leaveFunc;

	std::function<void()> sellUIResetFunc;

public:
	// デストラクタは実体を.cppに定義しておく（不完全型をメンバに持つため）
	~ShopPart();
	/*
	 * @brief 初期化
	 */
	void Init(UIManager* _uiManager, std::function<void()> _leaveFunc);

	/*
	 * @brief 引数に座標をもらって対応したJsonを開き、ショップに入る
	 */
	void Open(int _x, int _y,PlayerData* _currentTurnPlayer);

	/*
	 * @brief ショップを出る
	 */
	void Close();

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief 購入問い合わせ
	 */
	DontBuyReason BuyQuery(int _elementIndex);

	/*
	 * @brief 購入
	 */
	DontBuyReason Buy(int _elementIndex);

	/*
	 * @brief 売却
	 */
	void Sell(ElementID _elementID, int _id);

	void Sell(SellElement _sell);

	/*
	 * @brief ショップを出る
	 */
	void Leave();

	/*
	 * @brief 売れるアイテムを精査する
	 */
	void SearchSellElement();
};

#endif