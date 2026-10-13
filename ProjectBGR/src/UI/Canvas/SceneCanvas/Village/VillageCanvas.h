/*
 * @brief 村のキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _VILLAGECANVAS_H_
#define _VILLAGECANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/BoardGame/Village/VillageInfo.h"

class UIObject;
class UITextDialog;

class VillageCanvas : public UICanvasBase {
private:
	UITextDialog* dialog;
	UIObject* select;
	UIObject* tax;
	UIObject* upgrade;
	UIObject* acquisition;

	VillageNeedUIInfo info;

	VillageState prevVillageState;
public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;

	inline void SetVillageNeedUIInfo(const VillageNeedUIInfo& _info) { info = _info; }
private:
	void OpenGetTaxUI();

	void CloseGetTaxUI();

	void UpdateGetTaxUI();

	void OpenUpgradeUI();

	void CloseUpgradeUI();

	void UpdateUpgradeUI();

	void OpenAcquisitionUI();

	void CloseAcquisitionUI();

	void UpdateAcquisitionUI();
};

#endif