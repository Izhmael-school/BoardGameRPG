#include "UIBillboard.h"
#include "UI/UIObject/UIText/UIText.h"

UIBillboard::UIBillboard(Vector2 _pos, int _col, int _spaceX, int _spaceY, bool _isFitStringEnd, std::string _name)
	:UIObject(_name)
	, col(_col)
	, spaceX(_spaceX)
	, spaceY(_spaceY)
	, isFitStringEnd(_isFitStringEnd) {
	position = _pos;
}

UIBillboard::UIBillboard(std::string _name)
	:UIObject(_name)
	, col(0)
	, spaceX(0)
	, spaceY(0)
	, isFitStringEnd(false) {
	position = VZero_2;
}

void UIBillboard::OnUpdate(float _t, UIInput& _input) {
	std::vector<UIText*> txts = {};

	for (auto& child : children) {
		auto txt = static_cast<UIText*>(child.get());

		if (!txt)continue;

		txts.emplace_back(txt);
	}

	if (col <= 0) return;

	int firstVisibleRow = minCol > 0 ? minCol : 0;

	for (int i = 0; i < static_cast<int>(txts.size()); i++) {
		int c = i % col;
		int r = i / col;
		bool isVisible = r >= minCol && r <= maxCol;

		txts[i]->SetActive(isVisible);
		if (!isVisible) continue;

		Vector2 pos(0, 0);

		if (isFitStringEnd) {
			if (c > 0) {
				Vector2 endPos = txts[i - 1]->GetEndPosition();
				pos.x = endPos.x + spaceX;
				pos.y = txts[i - c]->GetPosition().y;
			}
			else if (r > firstVisibleRow) {
				// 前の表示行の末尾から次の行を配置
				Vector2 endPos = txts[i - col]->GetEndPosition();
				pos.y = endPos.y + spaceY;
			}
		}
		else {
			pos.x = c * spaceX;
			pos.y = (r - firstVisibleRow) * spaceY;
		}

		txts[i]->SetPosition(pos);
		txts[i]->RepositionX();
	}
}

void UIBillboard::SetSelect(int _selectIndex) {
	std::vector<UIText*> txts = {};

	int i = 0;
	for (auto& child : children) {
		auto txt = static_cast<UIText*>(child.get());

		if (!txt)continue;

		txt->SetSelect(i == _selectIndex);

		i++;
	}
}

void UIBillboard::SelectIndexCenterDrawRange(int _selectIndex, int _maxCol) {
	int half = _maxCol / 2;
	int size = GetUITextChildCount();

	if (size < _maxCol)return;

	if (_selectIndex < half)
		SetDrawRange(0, _maxCol - 1);
	else if (_selectIndex >= half && _selectIndex + half <= size - 1)
		SetDrawRange(_selectIndex - half, _selectIndex + half);
	else
		SetDrawRange(size - _maxCol, size);
}

int UIBillboard::GetUITextChildCount() {
	int count = 0;
	for (auto& c : children) {
		auto text = static_cast<UIText*>(c.get());

		if (!text) continue;

		count++;
	}
	return count;
}
