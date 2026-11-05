#include "PlayerSettingCanvas.h"
#include "UI/UIObject/UItext/UItext.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include <algorithm>

void PlayerSettingCanvas::Init() {
}

void PlayerSettingCanvas::Update(float _t, UIInput& _input) {
	PlayerSettingPart part = info.part;

	if (prevPart != part)
		switch (prevPart) {
		case PlayerSettingPart_PlayerNum:
			ClosePlayerNum();
			break;
		case PlayerSettingPart_PlayerName:
			ClosePlayerName();
			break;
		case PlayerSettingPart_UsePortSet:
			CloseUsePortSet();
			break;
		case PlayerSettingPart_PlayerJob:
			CloseJobSelectSet();
			break;
		case PlayerSettingPart_TurnOrder:
			CloseTurnOrderSet();
			break;
		}

	switch (part) {
	case PlayerSettingPart_PlayerNum:
		UpdatePlayerNum();
		break;
	case PlayerSettingPart_PlayerName:
		UpdatePlayerName();
		break;
	case PlayerSettingPart_UsePortSet:
		UpdateUsePortSet();
		break;
	case PlayerSettingPart_PlayerJob:
		UpdateJobSelectSet();
		break;
	case PlayerSettingPart_TurnOrder:
		UpdateTurnOrderSet();
		break;
	}
	prevPart = part;
}

void PlayerSettingCanvas::OpenPlayerNum() {
	// 既に開いていれば戻る
	if (playerNum) return;

	playerNum = Instantiate<UIObject>();

	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;

	int cw = 1920 / 2;

	auto titleText = UniqueInstantiate<UIText>("参加人数", titleStyle, Vector2(cw, 150));

	UITextStyle numStyle;
	numStyle.fontSize = 30;
	numStyle.textDrawPosition = TextDrawPosition_Center;
	auto playerText = UniqueInstantiate<UIText>("プレイヤーの数", numStyle, Vector2(cw, 400));
	auto playerNumText = UniqueInstantiate<UIText>("0", numStyle, Vector2(cw, 450));
	auto cpuText = UniqueInstantiate<UIText>("CPUの数", numStyle, Vector2(cw, 500));
	auto cpuNumText = UniqueInstantiate<UIText>("0", numStyle, Vector2(cw, 550));
	auto nextText = UniqueInstantiate<UIText>("決定", numStyle, Vector2(cw, 700));

	playerNum->AddChild(std::move(titleText));
	playerNum->AddChild(std::move(playerText));
	playerNum->AddChild(std::move(playerNumText));
	playerNum->AddChild(std::move(cpuText));
	playerNum->AddChild(std::move(cpuNumText));
	playerNum->AddChild(std::move(nextText));
}

void PlayerSettingCanvas::ClosePlayerNum() {
	DeleteUI(playerNum);
}

constexpr int PLAYERNUMTEXT_INDEX = 2;
constexpr int CPUNUMTEXT_INDEX = 4;
void PlayerSettingCanvas::UpdatePlayerNum() {
	if (!playerNum)
		OpenPlayerNum();

	auto& playerNumInfo = std::get<PlayerNumUINeedInfo>(info.info);
	auto children = playerNum->GetChildren();
	for (int i = 0, max = children.size(); i < max; i++) {
		auto text = static_cast<UIText*>(children[i]);

		if (!text) continue;

		if (1 + playerNumInfo.currentSelect * 2 != i)
			text->SetSelect(false);
		else
			text->SetSelect(true);

		if (i == PLAYERNUMTEXT_INDEX)
			text->SetText(std::to_string(playerNumInfo.playerNum));
		if (i == CPUNUMTEXT_INDEX)
			text->SetText(std::to_string(playerNumInfo.cpuNum));
	}
}

void PlayerSettingCanvas::OpenPlayerName() {
	if (playerName) return;

	playerName = Instantiate<UIObject>();

	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;

	int cw = 1920 / 2;

	auto titleText = UniqueInstantiate<UIText>("名前", titleStyle, Vector2(cw, 150), "TitleText");
	playerName->AddChild(std::move(titleText));

	UITextStyle nameStyle;
	nameStyle.textDrawPosition = TextDrawPosition_Center;
	nameStyle.fontSize = 35;

	auto dialog = UniqueInstantiate<UIText>("", nameStyle, Vector2(cw, 300), "NameDraw");
	playerName->AddChild(std::move(dialog));

	auto selectStringArray = UniqueInstantiate<UIObject>();

	// 文字の描画設定
	UITextStyle style;
	style.fontSize = 30;
	style.textDrawPosition = TextDrawPosition_Center;

	// 配置設定
	const int CELL_WIDTH = 80;
	const int CELL_HEIGHT = 60;

	// ひらがな一覧の列数・行数
	const int COL_COUNT = 10;
	const int ROW_COUNT = static_cast<int>(HIRAGANA.size());

	// 画面中央に一覧全体を配置
	const int START_X =
		1920 / 2 - (COL_COUNT - 1) * CELL_WIDTH / 2;

	const int START_Y =
		1080 / 1.7f - (ROW_COUNT - 1) * CELL_HEIGHT / 2;

	// ひらがなを順番に配置
	for (int row = 0; row < HIRAGANA.size(); row++) {
		for (int col = 0; col < HIRAGANA[row].size(); col++) {

			Vector2 pos(
				START_X + col * CELL_WIDTH,
				START_Y + row * CELL_HEIGHT
			);
			// 適当な全角文字で初期化
			auto text = UniqueInstantiate<UIText>(
				"あ", style, pos
			);

			selectStringArray->AddChild(std::move(text));
		}
	}
	playerName->AddChild(std::move(selectStringArray));
}

void PlayerSettingCanvas::ClosePlayerName() {
	DeleteUI(playerName);
}

void PlayerSettingCanvas::UpdatePlayerName() {
	if (!playerName)
		OpenPlayerName();

	auto& playerNameInfo =
		std::get<PlayerNameUINeedInfo>(info.info);

	// タイトル
	auto title = static_cast<UIText*>(playerName->GetChild("TitleText"));
	if (title) {
		std::string drawText = "プレイヤー" + std::to_string(playerNameInfo.playerNum + 1) + "の名前は？";
		title->SetText(drawText);
	}

	// 入力文字種に応じて一覧を選択
	const std::vector<std::vector<std::string>>* stringTable = nullptr;

	bool isChange = false;
	switch (playerNameInfo.inputString) {
	case InputString_Hiragana:
		stringTable = &HIRAGANA;
		isChange = true;
		break;

	case InputString_Katakana:
		stringTable = &KATAKANA;
		isChange = true;
		break;

	case InputString_English:
		stringTable = &ENGLISH;
		isChange = true;
		break;

	default:
		return;
	}

	auto children = playerName->GetChildren().back()->GetChildren();
	// 文字を変更していればすべての文字を消す
	if (isChange)
		for (auto child : children) {
			static_cast<UIText*>(child)->SetText("");
		}

	int childIndex = 0;

	for (int y = 0; y < stringTable->size(); ++y) {
		for (int x = 0; x < (*stringTable)[y].size(); ++x) {
			// 空文字のセルにはUIを作らない
			/*if ((*stringTable)[y][x].empty())
				continue;*/

				// UIの子要素数を超えないようにする
			if (childIndex >= children.size())
				return;

			auto text = static_cast<UIText*>(children[childIndex]);
			++childIndex;

			if (!text)
				continue;

			text->SetText((*stringTable)[y][x]);

			// 選択中の文字だけを選択状態にする
			text->SetSelect(
				x == playerNameInfo.currentX &&
				y == playerNameInfo.currentY
			);
		}
	}

	UIText* nameDraw = static_cast<UIText*>(playerName->GetChild("NameDraw"));

	if (nameDraw)
		nameDraw->SetText(playerNameInfo.name);
}

void PlayerSettingCanvas::OpenUsePortSet() {
	if (usePortSet) return;
	// 情報を取得できるまで生成しない
	auto& usePortSetInfo = std::get<UsePortSetUINeedInfo>(info.info);

	if (usePortSetInfo.playerNum == -1) return;

	usePortSet = Instantiate<UIObject>();

	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;

	int cw = 1920 / 2;

	auto titleText = UniqueInstantiate<UIText>("コントローラ設定", titleStyle, Vector2(cw, 150));
	usePortSet->AddChild(std::move(titleText));

	int x = 384;

	UITextStyle playerNameStyle;
	playerNameStyle.fontSize = 30;
	playerNameStyle.textDrawPosition = TextDrawPosition_Center;

	const auto& playerNameArray = usePortSetInfo.playerName;
	int size = playerNameArray.size();
	auto waiting = UniqueInstantiate<UIObject>("Waiting");
	for (int i = 0, max = usePortSetInfo.characterNum; i < max; i++) {
		std::string name;
		if (i < size)
			name = playerNameArray[i];
		else
			name = "CPU" + std::to_string(i - size + 1);

		auto playerName = UniqueInstantiate<UIText>(name, playerNameStyle, Vector2(x * (i + 1), 350));
		auto playerInputWaiting = UniqueInstantiate<UIText>("待機中", playerNameStyle, Vector2(x * (i + 1), 400));

		usePortSet->AddChild(std::move(playerName));
		waiting->AddChild(std::move(playerInputWaiting));
	}
	usePortSet->AddChild(std::move(waiting));
}

void PlayerSettingCanvas::CloseUsePortSet() {
	DeleteUI(usePortSet);
}

void PlayerSettingCanvas::UpdateUsePortSet() {
	if (!usePortSet)
		OpenUsePortSet();

	auto& usePortSetInfo = std::get<UsePortSetUINeedInfo>(info.info);

	auto waiting = usePortSet->GetChild("Waiting");

	if (waiting) {
		int index = usePortSetInfo.playerUsePortArray.size();
		auto children = waiting->GetChildren();
		for (int i = 0, max = usePortSetInfo.playerNum; i < max; i++) {
			auto text = static_cast<UIText*>(children[i]);

			std::string drawText = "";
			// 現在入力待機中のプレイヤーのUIの下に待機中と出す 
			if (i == index)
				drawText = "入力待機中";
			// 入力が終わったプレイヤーのUIの下にはコントローラのポート番号を出す
			else if (i < index)
				drawText = "コントローラ" + std::to_string(usePortSetInfo.playerUsePortArray[i] + 1);

			text->SetText(drawText);
		}
	}
}

void PlayerSettingCanvas::OpenJobSelectSet() {
	if (playerJob) return;

	// JSONからジョブ一覧を読み込む
	jobData = MyJson::LoadJsonFile(
		"res/ExternalFile/Data/OriginPlayerData.json"
	);

	if (!jobData.is_array() || jobData.empty())
		return;

	playerJob = Instantiate<UIObject>();

	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;
	int cw = 1920 / 2;
	auto titleText = UniqueInstantiate<UIText>("名前", titleStyle, Vector2(cw, 150), "TitleText");
	playerJob->AddChild(std::move(titleText));

	UITextStyle style;
	style.fontSize = 30;
	style.textDrawPosition = TextDrawPosition_Center;

	// 左側：ジョブ一覧
	const int LIST_X = 400;
	const int LIST_Y = 220;
	const int LIST_HEIGHT = 85;

	auto jobNameTexts = UniqueInstantiate<UIObject>("JobNameTexts");
	for (const auto& data : jobData) {
		std::string name = data["jobName"].get<std::string>();

		int y = jobNameTexts->GetChildrenCount() * LIST_HEIGHT;
		auto text = UniqueInstantiate<UIText>(name, style, Vector2(LIST_X, LIST_Y + y));

		jobNameTexts->AddChild(std::move(text));
	}
	playerJob->AddChild(std::move(jobNameTexts));

	// 右上：選択中のジョブ名
	auto selectJob = UniqueInstantiate<UIText>("", style, Vector2(1300, 150), "JobTitleText");

	playerJob->AddChild(std::move(selectJob));

	// 右側：ステータス
	const std::vector<std::string> statusNames = {
		"ATK", "DEF", "VIT", "MAG", "SPD", "LUK"
	};

	const int STATUS_X = 1050;
	const int STATUS_Y = 250;
	const int STATUS_HEIGHT = 55;

	auto jobStatusTexts = UniqueInstantiate<UIObject>("JobStatusTexts");
	for (int i = 0; i < statusNames.size(); ++i) {
		auto text = UniqueInstantiate<UIText>(
			statusNames[i] + ": 0",
			style,
			Vector2(STATUS_X, STATUS_Y + i * STATUS_HEIGHT)
		);

		jobStatusTexts->AddChild(std::move(text));
	}
	playerJob->AddChild(std::move(jobStatusTexts));

	// 下側：説明文
	auto description = UniqueInstantiate<UIText>("", style, Vector2(1250, 800), "JobDescriptionText");

	playerJob->AddChild(std::move(description));

	// CPUの選択
	int x = 320;
	int y = 270;

	auto cpuSelect = UniqueInstantiate<UIObject>("CPUSelect");
	auto bg = UniqueInstantiate<UISprite>(-1, Vector2(x, y), x * 4, y * 2);
	cpuSelect->AddChild(std::move(bg));
	UITextStyle cpuText;
	cpuText.textDrawPosition = TextDrawPosition_Center;
	cpuText.normalColor = 0x000000;
	auto text = UniqueInstantiate<UIText>("CPU0のジョブは", cpuText, Vector2(x * 3, y * 1.5f), "CPUName");
	auto options = UniqueInstantiate<UIObject>("Options");
	auto option1 = UniqueInstantiate<UIText>("ランダムで選ぶ", cpuText, Vector2(x * 2, y * 2.5f));
	auto option2 = UniqueInstantiate<UIText>("自分で選ぶ", cpuText, Vector2(x * 4, y * 2.5f));
	options->AddChild(std::move(option1));
	options->AddChild(std::move(option2));
	cpuSelect->AddChild(std::move(text));
	cpuSelect->AddChild(std::move(options));
	cpuSelect->SetActive(false);
	playerJob->AddChild(std::move(cpuSelect));
}

void PlayerSettingCanvas::CloseJobSelectSet() {
	DeleteUI(playerJob);
}

void PlayerSettingCanvas::UpdateJobSelectSet() {
	if (!playerJob)
		OpenJobSelectSet();

	if (!playerJob || jobData.empty())
		return;

	auto& jobSelectInfo =
		std::get<JobSelectUINeedInfo>(info.info);

	int currentJobIndex = jobSelectInfo.currentSelectJobIndex;

	const auto& data = jobData[currentJobIndex];

	// タイトル
	auto title = static_cast<UIText*>(playerJob->GetChild("TitleText"));
	if (title) {
		std::string drawText = jobSelectInfo.playerName[jobSelectInfo.currentPlayerIndex] + "のジョブは？";
		title->SetText(drawText);
	}

	// CPU
	auto cpuSelect = playerJob->GetChild("CPUSelect");
	if (jobSelectInfo.isCPU && !jobSelectInfo.isEndCPUSelect) {
		if (cpuSelect) {
			cpuSelect->SetActive(true);

			// CPUの名前
			auto text = static_cast<UIText*>(cpuSelect->GetChild("CPUName"));
			if (text) {
				std::string name = jobSelectInfo.playerName[jobSelectInfo.currentPlayerIndex];
				text->SetText(name + "のジョブは");
			}

			// 選択肢の選択反映
			auto options = cpuSelect->GetChild("Options")->GetChildren();
			int i = 0;
			for (auto& o : options) {
				auto text = static_cast<UIText*>(o);

				if (text)
					text->SetSelect(i == jobSelectInfo.cpuSelect);
				i++;
			}
		}
	}
	else {
		if (cpuSelect)
			cpuSelect->SetActive(false);
	}

	auto jobNameTexts = playerJob->GetChild("JobNameTexts");
	auto jobNameTextChildren = jobNameTexts->GetChildren();
	int jobNameTextCount = jobNameTextChildren.size();
	// 左側：選択状態
	for (int i = 0; i < jobNameTextCount; ++i) {
		auto text = static_cast<UIText*>(jobNameTextChildren[i]);

		if (text)
			text->SetSelect(i == currentJobIndex);
	}

	auto jobTitleText = static_cast<UIText*>(playerJob->GetChild("JobTitleText"));

	// 右上：ジョブ名
	if (jobTitleText) {
		jobTitleText->SetText(
			data["jobName"].get<std::string>()
		);
	}

	// 右側：ステータス
	const std::vector<std::string> statusNames = {
		"ATK", "DEF", "VIT", "MAG", "SPD", "LUK"
	};

	const std::vector<std::string> jsonKeys = {
		"atk", "def", "vit", "mag", "spd", "luk"
	};

	auto jobStatusTexts = playerJob->GetChild("JobStatusTexts");
	auto jobStatusTextChildren = jobStatusTexts->GetChildren();
	int jobStatusTextCount = jobStatusTextChildren.size();

	for (int i = 0; i < jobStatusTextCount; ++i) {
		auto text = static_cast<UIText*>(jobStatusTextChildren[i]);

		if (!text)
			continue;

		text->SetText(
			statusNames[i] + ": " +
			std::to_string(data[jsonKeys[i]].get<int>())
		);
	}
}

void PlayerSettingCanvas::OpenTurnOrderSet() {
	if (turnOrder) return;

	turnOrder = Instantiate<UIObject>();

	// タイトル
	UITextStyle titleStyle;
	titleStyle.textDrawPosition = TextDrawPosition_Center;
	titleStyle.fontSize = 50;
	int cw = 1920 / 2;
	auto titleText = UniqueInstantiate<UIText>("ターンの順番", titleStyle, Vector2(cw, 150), "TitleText");
	turnOrder->AddChild(std::move(titleText));


	auto& turnOrderInfo = std::get<TurnOrderUINeedInfo>(info.info);

	// 名前の描画
	auto name = UniqueInstantiate<UIObject>("CharacterName");
	float px = 384;
	float py = 300;
	UITextStyle nameStyle;
	nameStyle.textDrawPosition = TextDrawPosition_Center;
	nameStyle.fontSize = 30;
	for (int i = 0, max = turnOrderInfo.characterNum; i < max; i++) {
		auto text = UniqueInstantiate<UIText>(turnOrderInfo.characterNameArray[i], nameStyle, Vector2(px * (i + 1), py));
		name->AddChild(std::move(text));
	}
	turnOrder->AddChild(std::move(name));

	// 順番の描画
	auto order = UniqueInstantiate<UIObject>("Order");
	float ox = 384, oy = 340;
	for (int i = 0, max = turnOrderInfo.characterNum; i < max; i++) {
		auto text = UniqueInstantiate<UIText>("", nameStyle, Vector2(ox * (i + 1), oy));
		order->AddChild(std::move(text));
	}
	turnOrder->AddChild(std::move(order));

	// 順番の決め方の描画
	{
		auto select = UniqueInstantiate<UIObject>("Select");
		int x = 320, y = 270;
		auto bg = UniqueInstantiate<UISprite>(-1, Vector2(x, y), x * 4, y * 2);
		select->AddChild(std::move(bg));
		UITextStyle cpuText;
		cpuText.textDrawPosition = TextDrawPosition_Center;
		cpuText.normalColor = 0x000000;
		auto text = UniqueInstantiate<UIText>("順番の決め方は", cpuText, Vector2(x * 3, y * 1.5f));
		auto options = UniqueInstantiate<UIObject>("Options");
		auto option1 = UniqueInstantiate<UIText>("ランダムで決める", cpuText, Vector2(x * 2, y * 2.5f));
		auto option2 = UniqueInstantiate<UIText>("自分で決める", cpuText, Vector2(x * 4, y * 2.5f));
		options->AddChild(std::move(option1));
		options->AddChild(std::move(option2));
		select->AddChild(std::move(text));
		select->AddChild(std::move(options));
		select->SetActive(false);
		turnOrder->AddChild(std::move(select));
	}

	// 確認の描画
	{
		auto confirm = UniqueInstantiate<UIObject>("Confirm");
		int x = 320, y = 400;
		auto bg = UniqueInstantiate<UISprite>(-1, Vector2(x, y), x * 4, y * 2);
		confirm->AddChild(std::move(bg));
		UITextStyle cpuText;
		cpuText.textDrawPosition = TextDrawPosition_Center;
		cpuText.normalColor = 0x000000;
		auto text = UniqueInstantiate<UIText>("これでいいですか？", cpuText, Vector2(x * 3, y * 1.5f));
		auto options = UniqueInstantiate<UIObject>("Options");
		auto option1 = UniqueInstantiate<UIText>("いいえ", cpuText, Vector2(x * 2, y * 2.5f));
		auto option2 = UniqueInstantiate<UIText>("はい", cpuText, Vector2(x * 4, y * 2.5f));
		options->AddChild(std::move(option1));
		options->AddChild(std::move(option2));
		confirm->AddChild(std::move(text));
		confirm->AddChild(std::move(options));
		confirm->SetActive(false);
		turnOrder->AddChild(std::move(confirm));
	}
}

void PlayerSettingCanvas::CloseTurnOrderSet() {
	DeleteUI(turnOrder);
}

void PlayerSettingCanvas::UpdateTurnOrderSet() {
	if (!turnOrder)
		OpenTurnOrderSet();

	auto& turnOrderInfo = std::get<TurnOrderUINeedInfo>(info.info);

	auto confirm = turnOrder->GetChild("Confirm");
	if (confirm) {
		bool isConfirm = turnOrderInfo.currentSection == TurnOrderSection_Confirm;
		confirm->SetActive(isConfirm);

		auto children = confirm->GetChild("Options")->GetChildren();
		if (isConfirm) {
			for (int i = 0, max = 2; i < max; i++) {
				auto text = static_cast<UIText*>(children[i]);

				if (text)
					text->SetSelect(i == turnOrderInfo.confirm);
			}
		}
	}

	auto select = turnOrder->GetChild("Select");
	if (select){
		bool isSelect = turnOrderInfo.currentSection == TurnOrderSection_ModeSelect;
		select->SetActive(isSelect);

		auto children = select->GetChild("Options")->GetChildren();
		if (isSelect) {
			for (int i = 0, max = 2; i < max; i++) {
				auto text = static_cast<UIText*>(children[i]);

				if (text)
					text->SetSelect(i == turnOrderInfo.currentSelect);
			}
		}
	}
	// 選択中のプレイヤーを描画
	auto children = turnOrder->GetChild("CharacterName");
	auto characterNameChildren = children->GetChildren();
	for (int i = 0, max = turnOrderInfo.characterNum; i < max; i++) {
		auto text = static_cast<UIText*>(characterNameChildren[i]);

		if (text)
			text->SetSelect(i == turnOrderInfo.currentSelectCharacter);
	}

	// 順番を描画

	auto orderChildren = turnOrder->GetChild("Order")->GetChildren();

	for (int i = 0, max = turnOrderInfo.characterNum; i < max; i++) {
		auto text = static_cast<UIText*>(orderChildren[i]);

		if (text) {
			int order = turnOrderInfo.orderArray[i] + 1;
			if (order != 0)
				text->SetText(std::to_string(order) + "番");
			else
				text->SetText("");
		}
	}
}
