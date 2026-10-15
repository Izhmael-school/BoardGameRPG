/*
 * @brief ゲームに参加するプレイヤーの設定をするシーン
 * @author Sekino
 */
#pragma once
#ifndef _PLAYERSETTINGSCENE_H_
#define _PLAYERSETTINGSCENE_H_

#include "../SceneBase.h"
#include "Turn/Part/PlayerSetting/PlayerSettingInfo.h"
#include "Turn/Part/PlayerSetting/PlayerNum/DecidePlayerNumPart.h"
#include "Turn/Part/PlayerSetting/PlayerName/DecidePlayerNamePart.h"

class UIManager;

class PlayerSettingScene : public SceneBase {
private:
	PlayerSettingPart currentPart;
	UIManager* uiManager;
	DecidePlayerNumPart playerNum;
	DecidePlayerNamePart playerName;

public:
	PlayerSettingScene(UIManager* _uiManager);
	~PlayerSettingScene() override;

private:
	//void Start() override;

public:
	void Update(float _t) override;

	void Render() override;

	void Setup() override;

	void Cleanup() override;
private:

	/*
	 * @brief ゲームに参加するプレイヤーの人数決め
	 */
	void DecidePlayerNum();

	/*
	 * @brief 使うコントローラーの設定
	 */
	void DecideUsePortNum();

	/*
	 * @brief ゲームに参加するプレイヤーの名前決め
	 */
	void DecidePlayerName();

	/*
	 * @brief プレイヤーの行動順決め
	 */
	void DecideTurnOrder();

	/*
	 * @brief プレイヤーのジョブ決め
	 */
	void DecidePlayerJob();

	void ChangeSettingPart(PlayerSettingPart _part);
};

#endif // !_PLAYERSETTINGSCENE_H_