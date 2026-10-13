/*
 * @brief ショップ用キャンバス
 * @author Sekino
 */
#pragma once
#ifndef _SHOPCANVAS_H_
#define _SHOPCANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/BoardGame/Shop/ShopElement.h"
#include <vector>
#include <string>
#include <functional>

class UITextDialog;
class UIObject;
class BuyElement;
struct UIInput;

class ShopCanvas : public UICanvasBase {
private:
	UITextDialog* dialog;
	UIObject* select;
	UIObject* buy;
	UIObject* sell;

	std::vector<BuyElement> buyItems;

	int elementIndex = 0;

	int selectStateIndex = 0;

	ShopState shopState = ShopState_Selection;

	ShopState prevShopState = ShopState_Selection;

	std::string selectElementExplanation;

	DontBuyReason dontBuyReason;

	bool isDontBuyReasonTextVisible = false;

	std::function<std::vector<SellElement>()> getPlayerSellArrayFunc;

public:
	void Init()override;

	void Update(float _t, UIInput& _input) override;

	/*
	 * @brief 購入
	 */
	void OpenBuyUI();

	/*
	 * @brief 購入UIを消す
	 */
	void CloseBuyUI();

	/*
	 * @brief 購入UIの更新
	 */
	void UpdateBuyUI(float _t, UIInput& _input);

	/*
	 * @brief 売却UIの表示
	 */
	void OpenSellUI();

	/*
	 * @brief 売却UIを消す
	 */
	void CloseSellUI();

	/*
	 * @brief 売却UIの更新
	 */
	void UpdateSellUI(float _t, UIInput& _input);

	void SetBuyItems(const std::vector< BuyElement >& _items) { buyItems = _items; }

	void SetSelectVisible(bool _isVisible) { select->SetActive(_isVisible); }

	void SetShopState(ShopState _state) { shopState = _state; }

	void SetNeedInfo(const ShopUINeedInfo& _info) {
		prevShopState = shopState;
		shopState = _info.selection;
		selectStateIndex = _info.selectSectionIndex;
		elementIndex = _info.elementIndex;
		selectElementExplanation = _info.selectElement.elementExplanation;
		dontBuyReason = _info.dontBuyReason;
	}

	void SetGetPlayerSellArrayFunc(std::function<std::vector<SellElement>()> _func) { getPlayerSellArrayFunc = _func; }

	void ResetSellUI();
};


#endif // !_SHOPCANVAS_H_