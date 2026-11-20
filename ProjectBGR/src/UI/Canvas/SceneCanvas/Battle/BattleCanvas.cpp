#include "BattleCanvas.h"
#include "Turn/Part/Battle/BattleInfo.h"
#include "UI/UIObject/UIObject.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "UI/UIObject/UItext/UITextDialog/UITextDialog.h"
#include "UI/UIObject/UIWindow/UIWindow.h"
#include "UI/UIObject/UIGauge/UIGauge.h"
#include "String/MyString.h"
#include "Manager/Resource/Sprite/SpriteResourceManager.h"
#include "Application.h"
#include <format>
#include <string_view>
#include "Data/Player/PlayerData.h"
#include "UI/Input/UIInput.h"

void BattleCanvas::Init() {

	stateFunc.resize(static_cast<int>(BattleState_Max));
	auto attackOrder = &stateFunc[BattleState_AttackOrder];
	attackOrder->openUIFunc = [this]() {this->OpenAttackOrderUI(); };
	attackOrder->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateAttackOrderUI(_uiInput); };
	attackOrder->closeUIFunc = [this]() {this->CloseAttackOrderUI(); };
	auto selectCommand = &stateFunc[BattleState_SelectCommand];
	selectCommand->openUIFunc = [this]() {this->OpenSelectCommandUI(); };
	selectCommand->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateSelectCommandUI(_uiInput); };
	selectCommand->closeUIFunc = [this]() {this->DeleteUI(selectCommandUI); };
	auto attackConfirm = &stateFunc[BattleState_AttackConfirm];
	attackConfirm->openUIFunc = [this]() {this->OpenAttackConfirmUI(); };
	attackConfirm->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateAttackConfirmUI(_uiInput); };
	attackConfirm->closeUIFunc = [this]() {this->CloseAttackConfirmUI(); };
	auto turnChange = &stateFunc[BattleState_TurnChange];
	turnChange->openUIFunc = [this]() {this->OpenTurnChangeUI(); };
	turnChange->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateTurnChangeUI(_uiInput); };
	turnChange->closeUIFunc = [this]() {this->CloseTurnChangeUI(); };
	auto result = &stateFunc[BattleState_Result];
	result->openUIFunc = [this]() {this->OpenResultUI(); };
	result->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateResultUI(_uiInput); };
	result->closeUIFunc = [this]() {DeleteUI(resultUI); };
	auto levelup = &stateFunc[BattleState_LevelUp];
	levelup->openUIFunc = [this]() {this->OpenLevelUpUI(); };
	levelup->updateUIFunc = [this](UIInput& _uiInput) {this->UpdateLevelUpUI(_uiInput); };
	levelup->closeUIFunc = [this]() {DeleteUI(levelUpUI); isSendTextDialog = false; };
}

void BattleCanvas::Update(float _t, UIInput& _input) {
	currentState = info.state;

	UpdateAlwaysUI();

	if (currentState == BattleState_Invalid) return;

	// 前の状態のUIがあれば消す
	UICloseDifferentStateReason();

	if (currentState == BattleState_TurnEnd) {
		CleanUp();
		return;
	}

	// 更新
	stateFunc[currentState].updateUIFunc(_input);

	UICanvasBase::Update(_t, _input);
}

void BattleCanvas::UICloseDifferentStateReason() {

	// 状態が変わらなければ帰る
	if (currentState == prevState) return;

	// 閉じる
	if (prevState != BattleState_TurnEnd)
		stateFunc[prevState].closeUIFunc();

	prevState = currentState;
}

void BattleCanvas::OpenAttackOrderUI() {
	if (attackOrderUI) return;

	attackOrderUI = Instantiate<UIObject>();

	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	attackOrderUI->AddChild(std::move(dialog));
}

#include "Dxlib.h"
void BattleCanvas::UpdateAttackOrderUI(UIInput& _uiInput) {
	if (!attackOrderUI)
		OpenAttackOrderUI();

	auto attackOrderInfo = std::get_if<AttackOrderUINeedInfo>(&info.info);

	// 情報がなければ帰る
	if (!attackOrderInfo) return;

	attackCharacterIndex = attackOrderInfo->attackCharacterIndex;

	if (!isSendTextDialog) {
		auto dialog = static_cast<UITextDialog*>(attackOrderUI->GetChild("Dialog"));

		if (!dialog) return;


		std::string playerName = battleCharacters[attackOrderInfo->attackCharacterIndex]->GetPlayerName();

		std::string drawText = std::format("{0}の先攻！", playerName);
		dialog->AddText(drawText, [this] {return false; });

		isSendTextDialog = true;
	}
}

void BattleCanvas::CloseAttackOrderUI() {
	DeleteUI(attackOrderUI);
	isSendTextDialog = false;
}

void BattleCanvas::OpenSelectCommandUI() {
	if (selectCommandUI) return;

	auto selectCommandInfo = std::get_if<SelectCommandUINeedInfo>(&info.info);
	if (!selectCommandInfo) return;

	selectCommandUI = Instantiate<UIObject>();

	constexpr int FONT_SIZE = 50;
	constexpr int W = 120 + FONT_SIZE;
	constexpr int H = 110;
	constexpr int Y = 720;

	constexpr int LEFT_X = 480;
	constexpr int RIGHT_X = 480 * 3;

	int dPadSpriteHandle =
		Application::GetInstance().GetSpriteResourceManager()->GetSprite("DPad");

	UITextStyle commandTextStyle;
	commandTextStyle.textDrawPosition = TextDrawPosition_Center;
	commandTextStyle.fontSize = FONT_SIZE;
	commandTextStyle.edgeSize = 8;
	commandTextStyle.outlineColor = 0x000000;

	struct CommandLayout {
		size_t commandIndex;
		int offsetX;
		int offsetY;
	};

	// 十字キーとコマンドの生成
	auto addCommandPad = [&](int x,
		const std::vector<std::string>& commandNames,
		std::initializer_list<CommandLayout> layouts,
		std::vector<bool> _isCanSelectCommand) {

			auto commandList = UniqueInstantiate<UIObject>();

			auto dPad = UniqueInstantiate<UISprite>(
				dPadSpriteHandle,
				Vector2(x, Y),
				2.0f, 2.0f
			);

			commandList->AddChild(std::move(dPad));

			for (const auto& layout : layouts) {
				auto command = UniqueInstantiate<UIText>(
					commandNames.at(layout.commandIndex),
					commandTextStyle,
					Vector2(x + layout.offsetX, Y + layout.offsetY)
				);

				// コマンドが使えるか確認する
				command->SetCanSelect(_isCanSelectCommand[layout.commandIndex]);

				commandList->AddChild(std::move(command));
			}

			return commandList;
		};

	// 攻撃側のコマンド
	auto addOffenseCommand = [&](int x) {
		return addCommandPad(x, selectCommandInfo->offenseCommandName, {
			{ OffenseCommand_Attack,      W,  0 },
			{ OffenseCommand_Magic,       0, -H },
			{ OffenseCommand_FatalAttack, -W, 0 }
			}, selectCommandInfo->offenseCanCommandSelect);
		};

	// 防御側のコマンド
	auto addDefenseCommand = [&](int x) {
		return addCommandPad(x, selectCommandInfo->defenseCommandName, {
			{ DefenseCommand_Guard,      W,  0 },
			{ DefenseCommand_MagicGuard, 0, -H },
			{ DefenseCommand_Counter,   -W, 0 },
			{ DefenseCommand_Surrender,  0, H }
			}, selectCommandInfo->defenseCanCommandSelect);
		};


	// 攻守によって左右を入れ替える
	UIPtr offense = nullptr;
	UIPtr defense = nullptr;
	if (attackCharacterIndex == 1) {
		// 左側が防御、右側が攻撃
		offense = addOffenseCommand(RIGHT_X);
		defense = addDefenseCommand(LEFT_X);
	}
	else if (attackCharacterIndex == 0) {
		// 左側が攻撃、右側が防御
		offense = addOffenseCommand(LEFT_X);
		defense = addDefenseCommand(RIGHT_X);
	}
	offense->SetName("OffenseCommand");
	defense->SetName("DefenseCommand");
	selectCommandUI->AddChild(std::move(offense));
	selectCommandUI->AddChild(std::move(defense));
}

void BattleCanvas::UpdateSelectCommandUI(UIInput& _uiInput) {
	if (!selectCommandUI)
		OpenSelectCommandUI();

	auto selectCommandInfo = std::get_if<SelectCommandUINeedInfo>(&info.info);
	if (!selectCommandInfo) return;

	std::vector<bool> isSelectedArray = selectCommandInfo->isSelectedCommandArray;

	if (isSelectedArray[0]) {
		auto offense = selectCommandUI->GetChild("OffenseCommand");

		if (offense)
			offense->SetActive(false);
	}
	if (isSelectedArray[1]) {
		auto defense = selectCommandUI->GetChild("DefenseCommand");

		if (defense)
			defense->SetActive(false);
	}

}

void BattleCanvas::CloseSelectCommandUI() {
}

void BattleCanvas::OpenAttackConfirmUI() {
	if (attackConfirmUI) return;

	attackConfirmUI = Instantiate<UIObject>();

	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	attackConfirmUI->AddChild(std::move(dialog));
}

void BattleCanvas::UpdateAttackConfirmUI(UIInput& _uiInput) {
	if (!attackConfirmUI)
		OpenAttackConfirmUI();

	auto attackConfirmInfo = std::get_if<AttackConfirmUINeedInfo>(&info.info);
	if (!attackConfirmInfo) return;

	auto dialog = static_cast<UITextDialog*>(attackConfirmUI->GetChild("Dialog"));

	if (dialog && !isSendTextDialog) {
		std::string drawString = "";
		std::string offenseName = battleCharacters[attackCharacterIndex]->GetPlayerName();
		std::string defenseName = battleCharacters[attackCharacterIndex ^ 1]->GetPlayerName();
		std::string offenseActionName = attackConfirmInfo->offenseActionName;
		std::string defenseActionName = attackConfirmInfo->defenseActionName;
		int damageValue = attackConfirmInfo->damageValue;
		drawString = std::format("{0}の{1}！\n{2}は{3}をした！\n{4}のダメージ！", offenseName, offenseActionName, defenseName, defenseActionName, damageValue);
		dialog->AddText(drawString, []() {return false; });

		isSendTextDialog = true;
	}
}

void BattleCanvas::CloseAttackConfirmUI() {
	DeleteUI(attackConfirmUI);
	isSendTextDialog = false;
}

void BattleCanvas::OpenTurnChangeUI() {
	if (turnChangeUI) return;

	auto turnChangeInfo = std::get_if<TurnChangeUINeedInfo>(&info.info);
	if (!turnChangeInfo) return;

	turnChangeUI = Instantiate<UIObject>();

	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	turnChangeUI->AddChild(std::move(dialog));

	attackCharacterIndex = turnChangeInfo->newAttackCharacterIndex;
}

void BattleCanvas::UpdateTurnChangeUI(UIInput& _uiInput) {
	if (!turnChangeUI)
		OpenTurnChangeUI();

	if (!turnChangeUI) return;

	auto dialog = static_cast<UITextDialog*>(turnChangeUI->GetChild("Dialog"));

	if (dialog && !isSendTextDialog) {
		std::string drawString = "";
		std::string offenseName = battleCharacters[attackCharacterIndex]->GetPlayerName();
		drawString = std::format("攻守交替\n{0}の攻撃ターン！", offenseName);
		dialog->AddText(drawString, []() {return false; });

		isSendTextDialog = true;
	}
}

void BattleCanvas::CloseTurnChangeUI() {
	DeleteUI(turnChangeUI);
	isSendTextDialog = false;
}

void BattleCanvas::OpenResultUI() {
	if (resultUI) return;

	auto resultInfo = std::get_if<ResultUINeedInfo>(&info.info);
	if (!resultInfo) return;

	resultUI = Instantiate<UIObject>();

	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");

	std::string drawText = "";

	int winPlayer = resultInfo->winPlayer;

	if (winPlayer == -1)
		drawText = "引き分け！\n勝負は次のターンに持ち越し！";
	else {
		if(!resultInfo->isSurrender)
			drawText = std::format("{0}の勝利！\n{0}は{1}EXPと{2}ゴールドを手に入れた！", battleCharacters[winPlayer]->GetPlayerName(), resultInfo->getExp, resultInfo->getMoney);
		else
			drawText = std::format("{0}は降参した。{1}の勝利！\n{1}は{2}EXPと{3}ゴールドを手に入れた！\n{0}は{4}EXPと{5}ゴールドを失った。", battleCharacters[winPlayer ^ 1]->GetPlayerName(), battleCharacters[winPlayer]->GetPlayerName(), resultInfo->getExp, resultInfo->getMoney,resultInfo->removeExp,resultInfo->removeMoney);
	}

	dialog->AddText(drawText, []() {return false; });

	resultUI->AddChild(std::move(dialog));
}

void BattleCanvas::UpdateResultUI(UIInput& _uiInput) {
	if (!resultUI)
		OpenResultUI();
}

void BattleCanvas::CloseResultUI() {
}

void BattleCanvas::OpenLevelUpUI() {
	if (levelUpUI) return;

	auto levelUpInfo = std::get_if<LevelUpUINeedInfo>(&info.info);

	if (!levelUpInfo) return;

	levelUpUI = Instantiate<UIObject>();

	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");

	std::string playerName = levelUpInfo->levelUpPlayer->GetPlayerName();

	dialog->AddText(std::format("{0}はレベルが{1}上がった！", playerName, levelUpInfo->upLevelIndex), [this]() {
		auto isPressAGet = std::get_if<LevelUpUINeedInfo>(&info.info);
		if (!isPressAGet) return false;
		return isPressAGet->isPressA;
		});

	dialog->AddText("上昇させたいステータスに\nポイントを振り分けてください", [this]() {return false; });

	levelUpUI->AddChild(std::move(dialog));

	int fontSize = 60;

	UITextStyle statusTextStyle;
	statusTextStyle.normalColor = 0xffffff;
	statusTextStyle.fontSize = fontSize;
	statusTextStyle.textDrawPosition = TextDrawPosition_Center;

	int x = 1920 / 2;
	int y = (1080 / 7) * 3;

	auto statusText = UniqueInstantiate<UIObject>("StatusText");

	auto remainingPoint = UniqueInstantiate<UIText>("残り0ポイント", statusTextStyle, Vector2(x, y + (-3 * fontSize)), "RemainingPoint");
	for (int i = -2, max = 4; i < max; i++) {
		auto text = UniqueInstantiate<UIText>("XXX : X + X", statusTextStyle, Vector2(x, y + (i * fontSize)));
		statusText->AddChild(std::move(text));
	}

	levelUpUI->AddChild(std::move(remainingPoint));
	levelUpUI->AddChild(std::move(statusText));

	// 確定するかのUI
	int ox = 1200;
	int oy = 800;
	auto options = UniqueInstantiate<UIObject>("Options");
	auto option1 = UniqueInstantiate<UIText>("変える", explanation, Vector2(ox, oy));
	auto option2 = UniqueInstantiate<UIText>("確定", explanation, Vector2(ox, oy + fontSize));
	options->AddChild(std::move(option1));
	options->AddChild(std::move(option2));
	levelUpUI->AddChild(std::move(options));

	// 体力ゲージなどは消す
	DeleteUI(alwaysUI);
}

void BattleCanvas::UpdateLevelUpUI(UIInput& _uiInput) {
	if (!levelUpUI)
		OpenLevelUpUI();

	if (!levelUpUI)
		return;

	auto levelUpInfo = std::get_if<LevelUpUINeedInfo>(&info.info);

	if (!levelUpInfo) return;

	auto player = levelUpInfo->levelUpPlayer;

	if (!player) return;

	const std::vector<std::string> statusTextBase = { "ATK {} + {}","DEF {} + {}","VIT {} + {}","MAG {} + {}","SPD {} + {}","LUK {} + {}" };
	std::vector<int> playerStatus = player->GetAllStatus();
	std::vector<int> upStatus = levelUpInfo->upStatus;

	auto remainingPoint = static_cast<UIText*>(levelUpUI->GetChild("RemainingPoint"));

	if (remainingPoint)
		remainingPoint->SetText(std::format("残り{0}ポイント", levelUpInfo->remainingPoint));

	auto statusText = levelUpUI->GetChild("StatusText");

	if (statusText) {
		auto children = statusText->GetChildren();

		int index = 0;
		for (auto& child : children) {
			if (!child) continue;

			auto text = static_cast<UIText*>(child);

			if (!text) continue;

			std::string drawText = std::vformat(statusTextBase[index], std::make_format_args(playerStatus[index], upStatus[index]));

			text->SetText(drawText);

			text->SetSelect(levelUpInfo->currentSelectIndex == index);

			index++;
		}
	}

	auto option = levelUpUI->GetChild("Options");

	if (option) {
		bool isActive = levelUpInfo->isConfirm;

		option->SetActive(isActive);
		if (isActive) {

			int index = 0;
			for (auto& op : option->GetChildren()) {
				if (!op) continue;

				auto text = static_cast<UIText*>(op);

				if (!text) continue;

				text->SetSelect(index == levelUpInfo->confirmIndex);

				index++;
			}
		}
	}

	auto dialog = static_cast<UITextDialog*>(levelUpUI->GetChild("Dialog"));
	if (!dialog) return;

	if (levelUpInfo->isConfirm) {
		if (!isSendTextDialog) {
			dialog->DeleteFrontText();
			dialog->AddText("これで決定しますか？", []() {return false; });
			isSendTextDialog = true;
		}
	}
	else {
		if (isSendTextDialog) {
			dialog->DeleteFrontText();
			dialog->AddText("上昇させたいステータスに\nポイントを振り分けてください", []() {return false; });
			isSendTextDialog = false;
		}
	}
}

void BattleCanvas::OpenAlwaysUI() {
	if (alwaysUI || battleCharacters.empty()) return;

	alwaysUI = Instantiate<UIObject>();

	int x = 640;
	int y = 200;

	int colorL = attackCharacterIndex == 0 ? 0xff00008d : 0x0000ff8d;
	auto windowL = CreatePlayerInfo(Vector2(300, 200), colorL, battleCharacters[0], "WindowL");
	int colorR = attackCharacterIndex == 1 ? 0xff00008d : 0x0000ff8d;
	auto windowR = CreatePlayerInfo(Vector2(1620, 200), colorR, battleCharacters[1], "WindowR");
	alwaysUI->AddChild(std::move(windowL));
	alwaysUI->AddChild(std::move(windowR));
	auto status = CreatePlayerStatusInfo(Vector2(1920, 1080), battleCharacters);
	alwaysUI->AddChild(std::move(status));
}

void BattleCanvas::UpdateAlwaysUI() {
	if (currentState == BattleState_LevelUp) return;

	if (!alwaysUI)
		OpenAlwaysUI();

	if (!alwaysUI)
		return;

	auto windowL = static_cast<UIWindow*>(alwaysUI->GetChild("WindowL"));
	auto windowR = static_cast<UIWindow*>(alwaysUI->GetChild("WindowR"));

	auto gaugeL = static_cast<UIGauge*>(windowL->GetChild("Gauge"));
	auto gaugeR = static_cast<UIGauge*>(windowR->GetChild("Gauge"));
	UIGauge* gaugeUpdateArray[BATTLE_CHARACTER_NUM] = { gaugeL,gaugeR };

	auto hpValueL = static_cast<UIText*>(windowL->GetChild("HpValue"));
	auto hpValueR = static_cast<UIText*>(windowR->GetChild("HpValue"));
	UIText* hpValueUpdateArray[BATTLE_CHARACTER_NUM] = { hpValueL,hpValueR };

	for (int i = 0; i < BATTLE_CHARACTER_NUM; i++) {
		// キャラクターのデータが取得できなければ次
		auto chara = battleCharacters[i];
		if (!chara) continue;

		// ゲージの更新
		auto gauge = gaugeUpdateArray[i];
		int currentHP = chara->GetCurrentHp();
		int maxHP = chara->GetMaxHp();
		float v = (float)currentHP / (float)maxHP;
		if (gauge)
			gauge->SetValue(v);

		// HP値テキストの更新
		auto hpValue = hpValueUpdateArray[i];
		if (hpValue)
			hpValue->SetText(std::format("{0} / {1}", currentHP, maxHP));
	}
}

void BattleCanvas::CloseAlwaysUI() {
	DeleteUI(alwaysUI);
}

UIPtr BattleCanvas::CreatePlayerInfo(Vector2 _pos, int _color, CharacterData* _characterData, std::string _uiName) {
	if (!_characterData) return nullptr;

	// ウインドウの生成
	auto window = UniqueInstantiate<UIWindow>(_color, 0xffff00, _pos.x, _pos.y, 400, 150, true,10, _uiName);
	// ゲージの生成
	auto gauge = UniqueInstantiate<UIGauge>(Vector2(230, 120), 300, 20, 1.0f, DD_Right, "Gauge");
	// HPテキストの生成
	UITextStyle hpStyle;
	hpStyle.fontSize = 50;
	auto hp = UniqueInstantiate<UIText>("HP", hpStyle, Vector2(5, 120));
	// HP値テキストの生成
	UITextStyle hpValueStyle;
	hpValueStyle.fontSize = 20;
	hpValueStyle.textDrawPosition = TextDrawPosition_Right;
	std::string hpText = "";
	int currentHP = _characterData->GetCurrentHp();
	int maxHP = _characterData->GetMaxHp();
	hpText = std::format("{0} / {1}", currentHP, maxHP);
	auto hpValue = UniqueInstantiate<UIText>(hpText, hpValueStyle, Vector2(380, 142), "HpValue");
	// プレイヤーネームテキストの生成
	UITextStyle nameStyle;
	nameStyle.fontSize = 50;
	nameStyle.textDrawPosition = TextDrawPosition_Right;
	nameStyle.exRateX = 0.5f;
	std::string playerNameText = _characterData->GetPlayerName();
	auto playerName = UniqueInstantiate<UIText>(playerNameText, nameStyle, Vector2(380, 40));
	// レベルテキストの生成
	int lvValue = _characterData->GetLevel();
	std::string lvText = std::format("Lv{0}", lvValue);
	UITextStyle lvStyle;
	lvStyle.fontSize = 50;
	auto lv = UniqueInstantiate<UIText>(lvText, lvStyle, Vector2(20, 40));

	window->AddChild(std::move(gauge));
	window->AddChild(std::move(hp));
	window->AddChild(std::move(hpValue));
	window->AddChild(std::move(playerName));
	window->AddChild(std::move(lv));
	return window;
}

UIPtr BattleCanvas::CreatePlayerStatusInfo(Vector2 _pos, std::vector<CharacterData*> _characterDataArray, std::string _uiName) {

	if (_characterDataArray.empty()) return nullptr;

	constexpr int STATUS_NUM = 5;

	std::string_view statusText[STATUS_NUM] = { "Atk","Def","Mag","Spd","Luk" };

	int atk[2] = { -1,-1 };
	int def[2] = { -1,-1 };
	int mag[2] = { -1,-1 };
	int spd[2] = { -1,-1 };
	int luk[2] = { -1,-1 };

	// ステータスの取得
	int index = 0;
	for (auto& chara : _characterDataArray) {

		if (!chara) continue;

		atk[index] = chara->GetAtk();
		def[index] = chara->GetDef();
		mag[index] = chara->GetMag();
		spd[index] = chara->GetSpd();
		luk[index] = chara->GetLuk();

		index++;
	}

	int statusValue[STATUS_NUM][BATTLE_CHARACTER_NUM] = { {atk[0],atk[1]},{def[0],def[1]},{mag[0],mag[1]}, {spd[0],spd[1]}, {luk[0],luk[1]} };


	int fontSize = 50;
	int x = _pos.x / 2;
	int y = _pos.y / 2;

	// ステータス名テキストの生成
	UITextStyle statusTextStyle;
	statusTextStyle.textDrawPosition = TextDrawPosition_Center;
	statusTextStyle.fontSize = fontSize;
	UITextStyle leftStatusStyle;
	leftStatusStyle.textDrawPosition = TextDrawPosition_Right;
	leftStatusStyle.fontSize = fontSize;
	UITextStyle rightStatusStyle;
	rightStatusStyle.textDrawPosition = TextDrawPosition_Left;
	rightStatusStyle.fontSize = fontSize;
	UITextStyle statusStyle[BATTLE_CHARACTER_NUM] = { leftStatusStyle,rightStatusStyle };

	auto ui = UniqueInstantiate<UIObject>();
	for (int i = 0; i < STATUS_NUM; i++) {
		auto status = UniqueInstantiate<UIObject>();
		int index = i - 2;
		int dy = y + (fontSize * index);
		auto statusNametext = UniqueInstantiate<UIText>(statusText[i].data(), statusTextStyle, Vector2(x, dy));
		status->AddChild(std::move(statusNametext));
		for (int j = 0; j < BATTLE_CHARACTER_NUM; j++) {
			int jIndex = j == 0 ? -1 : j;
			auto pStatusText = UniqueInstantiate<UIText>(std::to_string(statusValue[i][j]), statusStyle[j], Vector2(x + (((fontSize / 2) * 3) * jIndex), dy));
			status->AddChild(std::move(pStatusText));
		}
		ui->AddChild(std::move(status));
	}


	return ui;
}

void BattleCanvas::CleanUp() {
	CloseAlwaysUI();
	battleCharacters.clear();
	attackCharacterIndex = -1;
	info.state = BattleState_Invalid;
}

