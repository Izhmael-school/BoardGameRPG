#include "PlayerSettingScene.h"
#include "UI/Canvas/SceneCanvas/PlayerSetting/PlayerSettingCanvas.h"
#include "Manager/UI/UIManager.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"

void PlayerSettingScene::ChangeSettingPart(PlayerSettingPart _part) {
	if (_part != PlayerSettingPart_Back)
		currentPart = _part;
	else
		// Backならひとつ前に戻る
		currentPart = static_cast<PlayerSettingPart>(currentPart - 1);
}

void PlayerSettingScene::CreateCharacterData() {
	std::vector<std::string> name = tempPlayerNameArray;
	std::vector<Job> job = jobSelect.GetPlayerJobArray();
	std::vector<int> port = usePortSet.GetPlayerUsePortArray();

	int cpuNum = playerNum.GetCPUNum();
	for (int i = 0, max = playerNum.GetCharaterNum(); i < max; i++) {
		auto chara = characterDataManager->CreatePlayerData(job[i]);
			chara->SetPlayerName(name[i]);

		if (i >= max - cpuNum) {
			chara->SetControllerNum(0);
			chara->SetCPU(true);
		}
		else {
			chara->SetControllerNum(port[i]);
			chara->SetCPU(false);
		}

		chara->SetPlayerNum(i);
	}
}

PlayerSettingScene::PlayerSettingScene(UIManager* _uiManager, CharacterDataManager* _characterDataManager, std::function<void(SceneType)> _changeSceneFunc, std::function<void(std::vector<int>)> _sendTurnOrderFunc)
	:uiManager(_uiManager)
	, characterDataManager(_characterDataManager)
	, currentPart(PlayerSettingPart_PlayerNum)
	, isGoNext(false)
	, changeSceneFunc(_changeSceneFunc)
	, sendTurnOrderFunc(_sendTurnOrderFunc) {
	sceneCanvas = std::make_unique<PlayerSettingCanvas>();

	std::function<void(PlayerSettingPart)> changeSettingPart = [this](PlayerSettingPart _part) {ChangeSettingPart(_part); };
	std::function<void(PlayerSettingUINeedInfo)> setNeedInfo = [this](PlayerSettingUINeedInfo _info) {static_cast<PlayerSettingCanvas*>(sceneCanvas.get())->SetNeedInfo(_info); };
	std::function<void()> goNext = [this]() {OnGoNext(); };

	playerNum.Init(changeSettingPart, setNeedInfo, goNext);
	playerName.Init(changeSettingPart, setNeedInfo, goNext);
	usePortSet.Init(changeSettingPart, setNeedInfo, goNext);
	jobSelect.Init(changeSettingPart, setNeedInfo, goNext);
	turnOrder.Init(changeSettingPart, setNeedInfo, goNext);
}

PlayerSettingScene::~PlayerSettingScene() {
}

void PlayerSettingScene::Update(float _t) {
#if _DEBUG

	if (InputManager::GetInstance().IsButtonDown(0, XINPUT_RIGHT_SHOULDER)) {
		characterDataManager->DebugCreatePlayerData();
		sendTurnOrderFunc({0,1,2,3});
		changeSceneFunc(MainGame);
	}

#endif

	switch (currentPart) {
	case PlayerSettingPart_Back:
		break;
	case PlayerSettingPart_PlayerNum:
		DecidePlayerNum();
		break;
	case PlayerSettingPart_PlayerName:
		DecidePlayerName();
		break;
	case PlayerSettingPart_UsePortSet:
		DecideUsePortNum();
		break;
	case PlayerSettingPart_PlayerJob:
		DecidePlayerJob();
		break;
	case PlayerSettingPart_TurnOrder:
		DecideTurnOrder();
		break;
	case PlayerSettingPart_Confirm:
		if (isCreatePlayer) break;
		CreateCharacterData();
		sendTurnOrderFunc(turnOrder.GetTurnOrder());
		changeSceneFunc(MainGame);
		isCreatePlayer = true;
		break;
	}
}

void PlayerSettingScene::Render() {
}

void PlayerSettingScene::Setup() {
	uiManager->PushCanvas(sceneCanvas.get());
	currentPart = PlayerSettingPart_PlayerNum;
	isCreatePlayer = false;
}

void PlayerSettingScene::Cleanup() {
	uiManager->PopCanvas(sceneCanvas.get());
	currentPart = PlayerSettingPart_Invalid;
}

void PlayerSettingScene::DecidePlayerNum() {
	playerNum.Update(0);

	if (isGoNext) {
		ChangeSettingPart(PlayerSettingPart_PlayerName);
		isGoNext = false;
	}
}

void PlayerSettingScene::DecideUsePortNum() {
	static bool isSetup = false;

	if (!isSetup) {
		usePortSet.Setup();
		// 必要な情報を渡す
		usePortSet.SetPlayerName(tempPlayerNameArray);
		usePortSet.SetPlayerNum(playerNum.GetPlayerNum());
		usePortSet.SetCharacterNum(playerNum.GetCharaterNum());
		isSetup = true;
	}

	usePortSet.Update(0);

	if (isGoNext) {
		ChangeSettingPart(PlayerSettingPart_PlayerJob);
		isGoNext = false;
	}
}

void PlayerSettingScene::DecidePlayerName() {
	static int correctNum = 0;
	playerName.Update(0);
	// 次に行くフラグが立ったら
	if (isGoNext) {
		// 名前を入れる
		// まだ名づけが終わってないプレイヤーがいれば次に進まず名づけ続行
		tempPlayerNameArray.emplace_back(playerName.GetName());
		playerName.Setup();

		// 一時的に名前を保存して次
		if (playerNum.GetPlayerNum() - 1 <= correctNum) {
			// 差分、CPUの名前を作る
			int i = playerNum.GetCharaterNum() - correctNum;
			if (i != 0)
				for (int j = 0; j < i; j++)
					tempPlayerNameArray.emplace_back(playerName.GetCPUName(j));

			// 人数分名づけをしたらコントローラ登録に進む
			ChangeSettingPart(PlayerSettingPart_UsePortSet);
			correctNum = 0;
		}

		correctNum++;
		isGoNext = false;
	}
}

void PlayerSettingScene::DecideTurnOrder() {
	static bool isSetup = false;

	if (!isSetup) {
		turnOrder.SetCharacterNum(playerNum.GetCharaterNum());
		turnOrder.Setup();
		// 必要な情報を渡す
		turnOrder.SetCharacterNameArray(tempPlayerNameArray);
		isSetup = true;
	}

	turnOrder.Update(0);

	if (isGoNext) {
		ChangeSettingPart(PlayerSettingPart_Confirm);
		isGoNext = false;
	}
}


void PlayerSettingScene::DecidePlayerJob() {

	static bool isSetup = false;

	if (!isSetup) {
		jobSelect.Setup();
		jobSelect.SetCharacterNum(playerNum.GetCharaterNum());
		jobSelect.SetPlayerNum(playerNum.GetPlayerNum());
		jobSelect.SetPlayerUsePortArray(usePortSet.GetPlayerUsePortArray());
		jobSelect.SetPlayerNameArray(tempPlayerNameArray);
		isSetup = true;
	}

	jobSelect.Update(0);

	if (isGoNext) {
		ChangeSettingPart(PlayerSettingPart_TurnOrder);

		isGoNext = false;
		isSetup = false;
	}
}
