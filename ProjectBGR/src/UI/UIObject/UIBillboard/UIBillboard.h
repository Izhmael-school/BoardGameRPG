/*
 * @brief 子供にあるUITextを縦横に羅列する
 */
#pragma once
#ifndef _UIBILLBOARD_H_
#define _UIBILLBOARD_H_

#include "../UIObject.h"
class UIBillboard : public UIObject {
private:
	int col;
	int spaceX;
	int spaceY;
	bool isFitStringEnd;
	int minCol;
	int maxCol;
public:
	UIBillboard(Vector2 _pos, int _col, int _spaceX, int _spaceY, bool _isFitStringEnd = true, std::string _name = "");
	UIBillboard(std::string _name);

	void OnUpdate(float _t, UIInput& _input) override;

	void SetSelect(int _selectIndex);

	void SetDrawRange(int _minCol, int _maxCol) { minCol = _minCol; maxCol = _maxCol; };

	void SelectIndexCenterDrawRange(int _selectIndex,int _maxCol);

	void SetFitStringEnd(bool _isFitStringEnd) { isFitStringEnd = _isFitStringEnd; }
	void SetColumn(int _col) { col = _col; }
	void SetSpace(int _x, int _y) { spaceX = _x; spaceY = _y; }
private:
	int GetUITextChildCount();
};

#endif // !_UIBILLBOARD_H_