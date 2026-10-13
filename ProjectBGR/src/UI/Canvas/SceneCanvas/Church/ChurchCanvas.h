/*
 * @brief 教会用キャンバス
 * @author Sekino
 */
#pragma once
#ifndef _CHURCHCANVAS_H_
#define _CHURCHCANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/BoardGame/Church/ChurchInfo.h"
#include <functional>

class UIObject;
class UITextDialog;

class ChurchCanvas : public UICanvasBase {
private:
	UITextDialog* dialog;
	UIObject* select;
	UIObject* warp;
	UIObject* setRP;
	UIObject* recovery;
	ChurchState churchState = ChurchState_Selection;
	ChurchState prevChurchState = ChurchState_Selection;
	DontReceiveServiceReason reason;
	std::function<std::vector<ChurchWarpNeedInfo>()> getCanWarpChurchsFunc;

	int selectStateIndex = 0;
	int selectCommandIndex = 0;
public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;

	void SetNeedInfo(ChurchUINeedInfo _info);

	void SetGetCanWarpChurchsFunc(std::function<std::vector<ChurchWarpNeedInfo>()> _func) { getCanWarpChurchsFunc = _func; }
private:
	void OpenRecoveryUI();

	void CloseRecoveryUI();

	void UpdateRecoveryUI();

	void OpenWarpUI();

	void CloseWarpUI();

	void UpdateWarpUI();

	void OpenSetRPUI();

	void CloseSetRPUI();

	void UpdateSetRPUI();


};

#endif