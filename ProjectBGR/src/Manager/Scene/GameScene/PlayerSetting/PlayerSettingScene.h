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
#include "Turn/Part/PlayerSetting/UsePortSet/DecideUsePortSetPart.h"
#include "Turn/Part/PlayerSetting/JobSelect/DecideJobSelectPart.h"
#include "Turn/Part/PlayerSetting/TurnOrder/DecideTurnOrderPart.h"
#include <string>
#include <vector>
#include <functional>
#include "../../SceneType.h"

class UIManager;
class CharacterDataManager;

class PlayerSettingScene : public SceneBase {
private:
	PlayerSettingPart currentPart;
	UIManager* uiManager;
	CharacterDataManager* characterDataManager;
	DecidePlayerNumPart playerNum;
	DecidePlayerNamePart playerName;
	DecideUsePortSetPart usePortSet;
	DecideJobSelectPart jobSelect;
	DecideTurnOrderPart turnOrder;

	bool isGoNext;
	std::vector<std::string> tempPlayerNameArray;

	std::function<void(SceneType)>  changeSceneFunc;
	std::function<void(std::vector<int>)> sendTurnOrderFunc;

	bool isCreatePlayer;
public:
	PlayerSettingScene(UIManager* _uiManager, CharacterDataManager* _characterDataManager,std::function<void(SceneType)> _changeSceneFunc,std::function<void(std::vector<int>)> _sendTurnOrderFunc);
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

	inline void OnGoNext() { isGoNext = true; };

	void CreateCharacterData();
};

#endif // !_PLAYERSETTINGSCENE_H_