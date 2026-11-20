#include "BoardGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDialog.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UISprite/UISprite.h"
#include "UI/UIObject/UIGauge/UIGauge.h"
#include "UI/UIObject/UIWindow/UIWindow.h"
#include "UI/UIObject/UISprite/UIRoulette/UIRoulette.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include "Manager/Resource/Sprite/SpriteResourceManager.h"
#include "Application.h"
#include "Manager/Map/MapInfo.h"
#include "String/MyString.h"
#include <format>
#include <algorithm>
#include "ExternalLibrary/ImGui/imgui.h"
#include <string_view>
#include "UI/UIContext.h"

void BoardGameCanvas::Init() {
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	explanationUI = Instantiate<UITextDialog>(explanation, Vector2(250, 700));
	UITextStyle playerName;
	playerName.fontSize = 50;
	playerNameUI = Instantiate<UIText>("PlayerName", playerName, Vector2(50, 100));
	moneyUI = Instantiate<UIText>("money", playerName, Vector2(50, 150));
	isDialogTextSend = false;
	contexts.resize(TurnState_Max);
}

void BoardGameCanvas::Update(float _t, UIInput& _input) {

	UIClose();

	switch (binfo.state) {
	case TurnState_CommandSelect:
		UpdateCommandSelectUI();
		break;
	case 	TurnState_Move:
		UpdateMoveCommandUI();
		break;
	case 	TurnState_Item:

		// アイテムリストの描画更新
		UpdateItemListUI();
		break;
	case 	TurnState_Magic:

		// マジックリストの描画更新
		UpdateMagicListUI();

		break;
	case 	TurnState_SearchTile:
		UpdateSearchTileUI();
		break;
	case 	TurnState_TileEffect:
		UpdateTileEffectUI();
		break;
	case 	TurnState_BattleIn:
		UpdateInBattleUI();
		break;
	default:
		break;
	}

	prevState = binfo.state;

	explanationUI->SetText("");

	playerNameUI->SetText(info.playerName);
	moneyUI->SetText(std::format("所持金  {0}Ｇ", info.money));

	UpdateAlwaysUI();

	UICanvasBase::Update(_t, _input);

}

void BoardGameCanvas::SetCurrentTurnPlayerData(PlayerData* _currentTurnPlayer) {
	PlayerData* p = _currentTurnPlayer;
	info.playerName = p->GetPlayerName();
	info.atk = p->GetAtk();
	info.def = p->GetDef();
	info.vit = p->GetVit();
	info.mag = p->GetMag();
	info.spd = p->GetSpd();
	info.luk = p->GetLuk();
	info.lv = p->GetLevel();
	info.maxHP = p->GetMaxHp();
	info.money = p->GetMoney();
	info.currentHP = p->GetCurrentHp();
	info.itemList = p->GetItemList();
	info.magicList = p->GetMagicList();
	info.equipList = p->GetEquipList();
}

void BoardGameCanvas::Setup() {
	CloseAlwaysUI();
}

void BoardGameCanvas::OpenItemListUI() {
	if (itemListUI) return;

	auto itemInfo = std::get_if<ItemCommandUINeedInfo>(&binfo.info);

	if (!itemInfo) return;

	int x = 400, y = 200, w = 1120, h = 480;
	itemListUI = Instantiate<UIObject>();
	auto window = UniqueInstantiate<UIWindow>(0x0000ff8d, 0xffff00, x, y, w, h, false, 10, "Window");
	itemListUI->AddChild(std::move(window));
	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	itemListUI->AddChild(std::move(dialog));
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle itemUIStyle;
	itemUIStyle.normalColor = 0x000000;
	itemUIStyle.fontSize = 50;
	auto text = UniqueInstantiate<UIObject>("Text");
	for (auto& item : info.itemList) {
		ItemData itemData = ElementDataManager::GetInstance().GetItemData(item);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int cx = (x + (itemUIStyle.fontSize * ((i + 1) % 2))) + ((((x + w) - x) / 2) * (i % 2));
		int cy = (y + itemUIStyle.fontSize) + (itemUIStyle.fontSize * (i / 2));

		Vector2 pPos = itemListUI->GetPosition();

		text->AddChild(UniqueInstantiate<UIText>(itemData.name, itemUIStyle, Vector2(cx - pPos.x, cy - pPos.y)));

		i++;
	}

	itemListUI->AddChild(std::move(text));
}

void BoardGameCanvas::CloseItemListUI() {
	DeleteUI(itemListUI);
	isDialogTextSend = false;
}

void BoardGameCanvas::UpdateItemListUI() {
	if (!itemListUI)
		OpenItemListUI();

	if (!itemListUI) return;

	auto itemInfo = std::get_if<ItemCommandUINeedInfo>(&binfo.info);

	int i = 0;
	static int prevSelect = -1;
	auto dialog = static_cast<UITextDialog*>(itemListUI->GetChild("Dialog"));
	int currentSelect = itemInfo->currentSelect;
	auto children = itemListUI->GetChild("Text")->GetChildren();

	bool isSelect = itemInfo->isSelect;
	bool isUsed = itemInfo->isUsed;
	static bool isSendSelectText = false;
	static bool isSendUseText = false;
	if (!isSelect && !isUsed) {

		for (auto& child : children) {
			UIText* text = static_cast<UIText*>(child);

			bool isSelect = i == currentSelect;
			text->SetSelect(isSelect);

			if (prevSelect != currentSelect) {
				prevSelect = currentSelect;
				isDialogTextSend = false;
				dialog->DeleteFrontText();
			}

			if (itemInfo->items.size() <= i) continue;

			std::string itemExplanation = ElementDataManager::GetInstance().GetItemData(itemInfo->items[currentSelect]).explanation;
			if (dialog && !isDialogTextSend) {
				dialog->AddText(itemExplanation, []() {return false; });
				isDialogTextSend = true;
			}
			i++;
		}
	}
	else if (isSelect && !isUsed) {
		if (!isSendSelectText) {
			dialog->DeleteFrontText();
			std::string itemName = ElementDataManager::GetInstance().GetItemData(itemInfo->items[currentSelect]).name;
			std::string drawText = std::format("{0}は{1}を使った！", info.playerName, itemName);
			dialog->AddText(drawText, [this]() { return false; });
			isSendSelectText = true;
			auto window = itemListUI->GetChild("Window");
			if (window)
				window->SetActive(false);
			auto text = itemListUI->GetChild("Text");
			if (text)
				text->SetActive(false);
		}
	}
	else if (isUsed) {
		if (!isSendUseText) {
			dialog->DeleteFrontText();
			std::string useExplanation = ElementDataManager::GetInstance().GetItemData(itemInfo->items[currentSelect]).useExplanation;
			std::string drawText = std::vformat(useExplanation, std::make_format_args(info.playerName, itemInfo->usedItemAnyInfo));
			dialog->AddText(drawText, [this]() { return false; });
			isSendUseText = true;
		}
	}

}

void BoardGameCanvas::OpenMagicListUI() {
	if (magicListUI) return;

	auto context = GetTurnUIContext(TurnState_Magic);
	auto player = *context->Get<PlayerData*>("currentTurnPlayer");
	auto& targets = *context->Get<std::vector<CharacterData*>>("targets");


	if (!player || targets.size() == 0) return;
	int x = 400, y = 200, w = 1120, h = 480;
	magicListUI = Instantiate<UIObject>();
	auto window = UniqueInstantiate<UIWindow>(0x0000ff8d, 0xffff00, x, y, w, h, false, 10, "WindowMagic");
	magicListUI->AddChild(std::move(window));
	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	magicListUI->AddChild(std::move(dialog));
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle magicUIStyle;
	magicUIStyle.normalColor = 0x000000;
	magicUIStyle.fontSize = 50;
	auto text = UniqueInstantiate<UIObject>("Text");

	auto& magics = player->GetMagicList();

	for (auto& magic : magics) {
		MagicData magicData = ElementDataManager::GetInstance().GetMagicData(magic);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int cx = (x + (magicUIStyle.fontSize * ((i + 1) % 2))) + ((((x + w) - x) / 2) * (i % 2));
		int cy = (y + magicUIStyle.fontSize) + (magicUIStyle.fontSize * (i / 2));

		Vector2 pPos = magicListUI->GetPosition();

		text->AddChild(UniqueInstantiate<UIText>(magicData.name, magicUIStyle, Vector2(cx - pPos.x, cy - pPos.y)));

		i++;
	}

	magicListUI->AddChild(std::move(text));


	constexpr int FONT_SIZE = 50;
	constexpr int MAX_CHAR_COUNT = 8;
	constexpr int MAX_COLUMN = 3;

	constexpr int CELL_WIDTH = FONT_SIZE * MAX_CHAR_COUNT;
	constexpr int CELL_HEIGHT = FONT_SIZE;

	// 横方向の間隔
	constexpr int INTERVAL_X = 50;

	// 縦方向の間隔
	constexpr int INTERVAL_Y = 30;

	// 1行分の幅
	constexpr int ROW_WIDTH =
		CELL_WIDTH * MAX_COLUMN + INTERVAL_X * (MAX_COLUMN - 1);
	magicUIStyle.textDrawPosition = TextDrawPosition_Center;
	auto targetUI = UniqueInstantiate<UIObject>("Targets");
	for (int i = 0; i < targets.size(); ++i) {

		// 列・行を計算
		int column = i % MAX_COLUMN;
		int row = i / MAX_COLUMN;

		// 中心座標を計算
		int centerX =
			1920 / 2
			+ column * (CELL_WIDTH + INTERVAL_X)
			- ROW_WIDTH / 2
			+ CELL_WIDTH / 2;

		int centerY =
			1080 / 5
			+ row * (CELL_HEIGHT + INTERVAL_Y)
			- CELL_HEIGHT / 2;

		// 文字を生成
		auto text = UniqueInstantiate<UIText>(targets[i]->GetPlayerName(), magicUIStyle, Vector2(centerX, centerY));
		targetUI->AddChild(std::move(text));
	}
	targetUI->SetActive(false);
	magicListUI->AddChild(std::move(targetUI));
}

void BoardGameCanvas::CloseMagicListUI() {
	if (!magicListUI) return;

	DeleteUI(magicListUI);

	isDialogTextSend = false;
}

void BoardGameCanvas::UpdateMagicListUI() {
	if (!magicListUI)
		OpenMagicListUI();

	if (!magicListUI) return;

	auto context = GetTurnUIContext(TurnState_Magic);

	auto children = magicListUI->GetChild("Text")->GetChildren();
	int i = 0;
	static int prev = -1;
	int selectMagicIndex = *context->Get<int>("selectMagicIndex");
	auto dialog = static_cast<UITextDialog*>(magicListUI->GetChild("Dialog"));
	static bool sendText = false;
	for (auto& child : children) {
		UIText* text = static_cast<UIText*>(child);

		bool isSelect = i == selectMagicIndex;

		text->SetSelect(isSelect);

		if (prev != selectMagicIndex) {
			sendText = false;
			dialog->DeleteFrontText();
			prev = selectMagicIndex;
		}

		if (isSelect && !sendText) {
			sendText = true;
			std::string magicExplanation = ElementDataManager::GetInstance().GetMagicData(info.magicList[i]).explanation;
			dialog->AddText(magicExplanation, []() {return false; });
		}

		i++;
	}

	auto targetUI = magicListUI->GetChild("Targets");
	auto magicUI = magicListUI->GetChild("Text");
	auto windowMagic = magicListUI->GetChild("WindowMagic");
	bool isTargetUIActive = *context->Get<bool>("IsSelectTarget");
	bool isMagicUIActive = *context->Get<bool>("IsSelectMagic");

	if (targetUI)
		targetUI->SetActive(isTargetUIActive);
	if (magicUI)
		magicUI->SetActive(isMagicUIActive);
	if (windowMagic)
		windowMagic->SetActive(isMagicUIActive);

	bool isUsed = *context->Get<bool>("IsUsed");
	auto player = *context->Get<PlayerData*>("currentTurnPlayer");
	if (isUsed && !isDialogTextSend) {
		isDialogTextSend = true;
		dialog->DeleteFrontText();
		std::string drawString = std::format("{0}は{1}を使った!", player->GetPlayerName(), *context->Get<std::string>("MagicName"));
		dialog->AddText(drawString, []() {return false; });
	}
}

const Vector2 ROULETTE_POS_3[] = {
	{ 960, 268 },
	{ 672, 812 },
	{ 1248, 812 }
};

void BoardGameCanvas::OpenMoveCommandUI() {
	if (moveCommandUI) return;

	auto rouletteInfo = std::get_if<MoveCommandUINeedInfo>(&binfo.info);

	if (!rouletteInfo) return;

	moveCommandUI = Instantiate<UIObject>();

	auto spriteManager = Application::GetInstance().GetSpriteResourceManager();
	int rouletteHandle = spriteManager->GetSprite("Roulette");
	int arrowHandle = spriteManager->GetSprite("RouletteArrow");

	int w = 1920;
	int h = 1080;
	// ルーレットの生成
	int rouletteNum = rouletteInfo->rouletteNum;
	auto roulette = UniqueInstantiate<UIObject>("Roulette");
	for (int i = 0; i < rouletteNum; i++) {
		auto r = UniqueInstantiate<UIRoulette>(rouletteHandle, arrowHandle, Vector2(w / 2, (h / 3) * 2), 1, 1, std::to_string(rouletteNum));
		if (rouletteNum == 2)
			r->SetPosition(ROULETTE_POS_3[i + 1]);
		else if (rouletteNum == 3)
			r->SetPosition(ROULETTE_POS_3[i]);
		roulette->AddChild(std::move(r));
	}

	moveCommandUI->AddChild(std::move(roulette));

	// 左角に進めるマスを表示
	int x = (1920 / 5);
	int y = (1080 / 5) * 4;

	UITextStyle canMoveTextStyle;
	canMoveTextStyle.fontSize = 70;
	canMoveTextStyle.normalColor = 0xffff00;
	auto canMoveTextWindow = UniqueInstantiate<UIWindow>("残り0マス", canMoveTextStyle, 0x008bff, 0xffff00, x, y, 10, "CanMoveText");
	moveCommandUI->AddChild(std::move(canMoveTextWindow));

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	moveCommandUI->AddChild(std::move(dialog));

	// 止まるかどうかのUI
	int ox = 250;
	int oy = 700;
	auto options = UniqueInstantiate<UIObject>("Options");
	auto option1 = UniqueInstantiate<UIText>("戻る", explanation, Vector2(x + 750, y));
	auto option2 = UniqueInstantiate<UIText>("止まる", explanation, Vector2(x + 750, y + 50));
	options->AddChild(std::move(option1));
	options->AddChild(std::move(option2));
	moveCommandUI->AddChild(std::move(options));
}

void BoardGameCanvas::UpdateMoveCommandUI() {
	if (!moveCommandUI)
		OpenMoveCommandUI();

	if (!moveCommandUI)return;

	auto rouletteInfo = std::get_if<MoveCommandUINeedInfo>(&binfo.info);

	if (!rouletteInfo) return;

	auto roulette = moveCommandUI->GetChild("Roulette");
	auto canMoveText = static_cast<UIWindow*>(moveCommandUI->GetChild("CanMoveText"));
	auto options = moveCommandUI->GetChild("Options");
	auto dialogUI = static_cast<UITextDialog*>(moveCommandUI->GetChild("Dialog"));
	if (rouletteInfo->isRouletteStop) {
		if (roulette)
			roulette->SetActive(false);

		int canMoveCount = rouletteInfo->canMoveCount;
		if (canMoveText) {
			canMoveText->SetActive(true);
			canMoveText->SetText(std::format("残り{0}マス", canMoveCount));
		}

		if (canMoveCount == 0) {
			// ここに何もしない処理を追加
			if (!isDialogTextSend) {

				if (dialogUI) {
					dialogUI->AddText("ここに止まりますか？", [this]() { return  std::get<MoveCommandUINeedInfo>(binfo.info).isDialogTextErase; });
					dialogUI->SetActive(true);
				}
				isDialogTextSend = true;
			}

			if (options) {

				options->SetActive(true);

				auto children = options->GetChildren();

				for (int i = 0; i < children.size(); ++i) {
					auto text = static_cast<UIText*>(children[i]);

					text->SetSelect(rouletteInfo->confirmationMove == i);
				}
			}
		}
		else {
			isDialogTextSend = false;
			options->SetActive(false);
		}
	}
	// ルーレットが回ってる状態
	else {
		if (roulette) {
			auto roulettes = roulette->GetChildren();

			int index = 0;
			for (auto robj : roulettes) {
				auto r = static_cast<UIRoulette*>(robj);
				if (r)
					r->SetArrowAngle(rouletteInfo->angle[index]);

				index++;
			}
			roulette->SetActive(true);
		}

		if (options)
			options->SetActive(false);

		if (canMoveText)
			canMoveText->SetActive(false);

		if (dialogUI)
			dialogUI->SetActive(false);
	}
}

void BoardGameCanvas::CloseMoveCommandUI() {
	DeleteUI(moveCommandUI);
	isDialogTextSend = false;
}

void BoardGameCanvas::OpenCommandSelectUI() {
	if (commandSelectUI) return;

	auto commandSelectInfo = std::get_if<CommandSelectUINeedInfo>(&binfo.info);

	if (!commandSelectInfo)return;

	commandSelectUI = Instantiate<UIObject>();

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	commandSelectUI->AddChild(std::move(dialog));

	auto options = UniqueInstantiate<UIObject>("Options");

	// コマンドの設定

	int fontSize = explanation.fontSize;
	int x = 250;
	int y = 350;
	auto& commandList = commandSelectInfo->commandList;
	auto window = UniqueInstantiate<UIWindow>(0x0000ff8d, 0xffff00, x + fontSize, y + (fontSize * 1.5f), fontSize * 2, fontSize * commandList.size());
	commandSelectUI->AddChild(std::move(window));

	int index = 0;
	for (auto& command : commandList) {
		auto option = UniqueInstantiate<UIText>(command.name, explanation, Vector2(x, y + (fontSize * index)));
		options->AddChild(std::move(option));
		index++;
	}
	commandSelectUI->AddChild(std::move(options));
}

void BoardGameCanvas::UpdateCommandSelectUI() {
	if (!commandSelectUI)
		OpenCommandSelectUI();

	auto commandSelectInfo = std::get_if<CommandSelectUINeedInfo>(&binfo.info);

	if (!commandSelectInfo) return;

	auto options = commandSelectUI->GetChild("Options");

	if (!options) return;

	auto children = options->GetChildren();

	// １フレーム前に選択していたコマンドの値
	static int prevSelectIndex = -1;
	// 今選択しているコマンドの値
	int currentSelectIndex = commandSelectInfo->selectIndex;

	auto dialogUI = static_cast<UITextDialog*>(commandSelectUI->GetChild("Dialog"));
	if (currentSelectIndex != prevSelectIndex) {
		isDialogTextSend = false;
		if (dialogUI)
			dialogUI->DeleteFrontText();
	}

	for (int i = 0, max = children.size(); i < max; ++i) {
		auto text = static_cast<UIText*>(children[i]);

		bool isSelected = (currentSelectIndex == i);

		text->SetSelect(isSelected);

		if (!isSelected) continue;

		// 選択されているコマンドの説明を表示

		if (!dialogUI || isDialogTextSend) return;

		dialogUI->SetActive(true);

		dialogUI->AddText(commandSelectInfo->commandList[i].explanation, []() { return  false; });

		isDialogTextSend = true;
	}

	prevSelectIndex = currentSelectIndex;
}

void BoardGameCanvas::CloseCommandSelectUI() {
	DeleteUI(commandSelectUI);
	isDialogTextSend = false;
}

void BoardGameCanvas::OpenSearchTileUI() {
	if (searchTileUI) return;

	searchTileUI = Instantiate<UIObject>();

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	searchTileUI->AddChild(std::move(dialog));
}

void BoardGameCanvas::UpdateSearchTileUI() {
	if (!searchTileUI)
		OpenSearchTileUI();

	auto searchTileInfo = std::get_if<SearchTileUINeedInfo>(&binfo.info);

	if (!searchTileInfo) return;

	auto tileType = searchTileInfo->tileType;

	static MapTileType prevTileType = Tile_Empty;

	if (tileType == prevTileType) return;

	std::string tileTypeText;

	switch (tileType) {
	case Tile_Empty:
		tileTypeText = "何もないマス。戦闘がおこるかも？";
		break;
	case Tile_Item:
		tileTypeText = "止まるとアイテムがランダムでもらえる";
		break;
	case Tile_Magic:
		tileTypeText = "止まると魔法ががランダムでもらえる";
		break;
	case Tile_ItemShop:
		tileTypeText = "アイテムを購入できる";
		break;
	case Tile_MagicShop:
		tileTypeText = "魔法を購入できる";
		break;
	case Tile_EquipShop:
		tileTypeText = "装備を購入できる";
		break;
	case Tile_Village:
		tileTypeText = "領主になると税がもらえる";
		break;
	case Tile_Church:
		tileTypeText = "回復や復活地点の変更ができる";
		break;
	case Tile_Castle:
		break;
	default:
		break;
	}

	auto dialogUI = static_cast<UITextDialog*>(searchTileUI->GetChild("Dialog"));

	if (!dialogUI) return;

	dialogUI->SetActive(true);
	dialogUI->DeleteFrontText();
	dialogUI->AddText(tileTypeText, []() { return false; });

	prevTileType = tileType;
}

void BoardGameCanvas::CloseSearchTileUI() {
	DeleteUI(searchTileUI);
}

void BoardGameCanvas::OpenInBattleUI() {
	if (inBattleUI) return;

	auto inBattleInfo = std::get_if<BattleInUINeedInfo>(&binfo.info);

	if (!inBattleInfo) return;

	inBattleUI = Instantiate<UIObject>();

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	inBattleUI->AddChild(std::move(dialog));

	auto& battleTargets = inBattleInfo->battleTargets;

	auto options = UniqueInstantiate<UIObject>("Options");

	for (int i = 0, max = battleTargets.size(); i < max; ++i) {
		int x = 250;
		int y = 200 + explanation.fontSize * i;
		auto option = UniqueInstantiate<UIText>(battleTargets[i]->GetPlayerName(), explanation, Vector2(x, y));
		options->AddChild(std::move(option));
	}
	inBattleUI->AddChild(std::move(options));
}

void BoardGameCanvas::UpdateInBattleUI() {
	if (!inBattleUI)
		OpenInBattleUI();

	auto inBattleInfo = std::get_if<BattleInUINeedInfo>(&binfo.info);

	if (!inBattleInfo) return;

	auto dialogUI = static_cast<UITextDialog*>(inBattleUI->GetChild("Dialog"));

	if (dialogUI && !isDialogTextSend) {
		dialogUI->SetActive(true);
		dialogUI->AddText("誰と戦いますか？", []() { return false; });
		isDialogTextSend = true;
	}

	auto options = inBattleUI->GetChild("Options");

	if (options) {
		auto children = options->GetChildren();
		for (int i = 0, max = children.size(); i < max; ++i) {
			auto text = static_cast<UIText*>(children[i]);
			text->SetSelect(inBattleInfo->selectIndex == i);
		}
	}
}

void BoardGameCanvas::CloseInBattleUI() {
	DeleteUI(inBattleUI);
	isDialogTextSend = false;
}

void BoardGameCanvas::OpenTileEffectUI() {
	if (tileEffectUI) return;

	auto tileEffectInfo = std::get_if<TileEffectUINeedInfo>(&binfo.info);

	if (!tileEffectInfo) return;

	tileEffectUI = Instantiate<UIObject>();

	auto options = UniqueInstantiate<UIObject>("Options");

	UITextStyle textStyle;
	textStyle.textDrawPosition = TextDrawPosition_Center;
	textStyle.fontSize = 50;

	std::vector<int> indexes = { 0,1,2,3,4,5,6,7,8,9 };
	int w = 480;
	int w2 = 320;
	int h = 135;
	const Vector2 windowPosition[10] = {
	Vector2(w,h),
	Vector2(w * 2, h),
	Vector2(w * 3,h),
	Vector2(w2 * 2, h * 2),
	Vector2(w2 * 4, h * 2),
	Vector2(w , h * 3),
	Vector2(w * 2 , h * 3),
	Vector2(w * 3 , h * 3),
	Vector2(w2 * 2, h * 4),
	Vector2(w2 * 4, h * 4)
	};
	int x = 1920 / 2;
	int y = 1080 / 10;
	for (int i = 0, max = indexes.size(); i < max; ++i) {
		int rand = MyMath::Random(0, indexes.size() - 1);
		indexes.erase(indexes.begin() + rand);
		Vector2 pos = windowPosition[i];
		auto window = UniqueInstantiate<UIWindow>(tileEffectInfo->elementNameList[i], textStyle, 0x008bff, 0xffff00, pos.x, pos.y, 10, std::format("{0}", i));
		options->AddChild(std::move(window));
	}
	tileEffectUI->AddChild(std::move(options));

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	tileEffectUI->AddChild(std::move(dialog));
}

void BoardGameCanvas::UpdateTileEffectUI() {
	if (!tileEffectUI)
		OpenTileEffectUI();

	auto tileEffectInfo = std::get_if<TileEffectUINeedInfo>(&binfo.info);

	if (!tileEffectInfo) return;

	auto options = tileEffectUI->GetChild("Options");

	for (auto o : options->GetChildren()) {
		std::string name = o->GetName();

		auto window = static_cast<UIWindow*>(o);

		if (!window) continue;
		window->GetTextUI()->SetSelect(name == std::format("{0}", tileEffectInfo->currentSelect));
	}

	if (!tileEffectInfo->isStop) return;

	auto dialogUI = static_cast<UITextDialog*>(tileEffectUI->GetChild("Dialog"));

	if (dialogUI && !isDialogTextSend) {
		dialogUI->SetActive(true);
		dialogUI->AddText(MyString::MergeString(tileEffectInfo->elementNameList[tileEffectInfo->currentSelect], "を手に入れた！"), []() { return false; });
		isDialogTextSend = true;
	}
}

void BoardGameCanvas::CloseTileEffectUI() {
	DeleteUI(tileEffectUI);
	isDialogTextSend = false;
}

const std::vector<std::string> ICON_NAME = { "ATK","DEF", "MAG","SPD", "LUK" };
constexpr std::string_view ICON_FORMAT = "Icon_{0}";

void BoardGameCanvas::OpenAlwaysUI() {
	if (alwaysUI) return;

	alwaysUI = Instantiate<UIObject>();

	int statusBannerHandle = Application::GetInstance().GetSpriteResourceManager()->GetSprite("StatusBanner");
	auto statusBanner = UniqueInstantiate<UISprite>(statusBannerHandle, Vector2(1920 / 2, 175), 0.8f, 0.7f, 0, "Status");

	UITextStyle statusTextStyle;
	statusTextStyle.fontSize = 70;

	int x = 800;
	int y = -45;

	auto levelText = UniqueInstantiate<UIText>(std::format("LV {0}", info.lv), statusTextStyle, Vector2(-x + 70, y), "Level");
	statusBanner->AddChild(std::move(levelText));
	auto playerNameText = UniqueInstantiate<UIText>(info.playerName, statusTextStyle, Vector2(-x + 370, y));
	statusBanner->AddChild(std::move(playerNameText));
	statusTextStyle.textDrawPosition = TextDrawPosition_Right;
	auto moneyText = UniqueInstantiate<UIText>(std::format("{0}Ｇ", info.money), statusTextStyle, Vector2(x - 70, y), "Money");
	statusTextStyle.textDrawPosition = TextDrawPosition_Left;
	statusBanner->AddChild(std::move(moneyText));
	SpriteResourceManager* spriteManager = Application::GetInstance().GetSpriteResourceManager();

	x = -700;
	y = 30;

	std::vector<int> status = { info.atk, info.def, info.mag, info.spd, info.luk };
	statusTextStyle.textDrawPosition = TextDrawPosition_Right;
	// ステータス
	for (int i = 0, max = 5; i < max; i++) {
		std::string fileName = std::format(ICON_FORMAT, ICON_NAME[i]);
		int graphHandle = spriteManager->GetSprite(fileName);
		if (graphHandle == -1) continue;
		auto icon = UniqueInstantiate<UISprite>(graphHandle, Vector2(x + (200 * i), y), 1.1f, 1.1f);
		statusBanner->AddChild(std::move(icon));
		auto text = UniqueInstantiate<UIText>(std::to_string(status[i]), statusTextStyle, Vector2(x + (200 * i) + 100, y), ICON_NAME[i]);
		statusBanner->AddChild(std::move(text));
	}

	x = 300;

	float value = info.currentHP / info.maxHP;
	std::string fileName = std::format(ICON_FORMAT, "HP");
	int graphHandle = spriteManager->GetSprite(fileName);
	auto hpIcon = UniqueInstantiate<UISprite>(graphHandle, Vector2(x, y), 1.1f, 1.1f);
	statusBanner->AddChild(std::move(hpIcon));
	x = 530;
	statusTextStyle.fontSize = 30;
	auto hpBar = UniqueInstantiate<UIGauge>(Vector2(x, y), 350, 20, value, DD_Right, "HPGauge");
	statusBanner->AddChild(std::move(hpBar));
	x = 700;
	y = 60;
	auto hpText = UniqueInstantiate<UIText>(std::format("{0} / {1}", info.currentHP, info.maxHP), statusTextStyle, Vector2(x, y), "HP");
	statusBanner->AddChild(std::move(hpText));

	alwaysUI->AddChild(std::move(statusBanner));
}

void BoardGameCanvas::UpdateAlwaysUI() {
	if (!alwaysUI)
		OpenAlwaysUI();

	// ステータスの更新
	auto status = alwaysUI->GetChild("Status");

	if (!status) return;

	auto level = static_cast<UIText*>(status->GetChild("Level"));
	if (level)
		level->SetText(std::format("LV {0}", info.lv));

	auto money = static_cast<UIText*>(status->GetChild("Money"));
	if (money)
		money->SetText(std::format("{0}Ｇ", info.money));

	auto atk = static_cast<UIText*>(status->GetChild("ATK"));
	if (atk)
		atk->SetText(std::to_string(info.atk));

	auto def = static_cast<UIText*>(status->GetChild("DEF"));
	if (def)
		def->SetText(std::to_string(info.def));

	auto mag = static_cast<UIText*>(status->GetChild("MAG"));
	if (mag)
		mag->SetText(std::to_string(info.mag));

	auto spd = static_cast<UIText*>(status->GetChild("SPD"));
	if (spd)
		spd->SetText(std::to_string(info.spd));

	auto luk = static_cast<UIText*>(status->GetChild("LUK"));
	if (luk)
		luk->SetText(std::to_string(info.luk));

	auto hp = static_cast<UIText*>(status->GetChild("HP"));
	if (hp)
		hp->SetText(std::format("{0} / {1}", info.currentHP, info.maxHP));
}

void BoardGameCanvas::CloseAlwaysUI() {
	DeleteUI(alwaysUI);
}

void BoardGameCanvas::UIClose() {
	if (binfo.state == prevState) return;

	switch (prevState) {
	case TurnState_CommandSelect:
		CloseCommandSelectUI();
		break;
	case TurnState_Move:
		CloseMoveCommandUI();
		break;
	case TurnState_Item:
		CloseItemListUI();
		break;
	case TurnState_Magic:
		CloseMagicListUI();
		break;
	case TurnState_SearchTile:
		CloseSearchTileUI();
		break;
	case TurnState_TileEffect:
		CloseTileEffectUI();
		break;
	case TurnState_BattleIn:
		CloseInBattleUI();
		break;
	case TurnState_End:
		CloseAlwaysUI();
		break;
	default:
		break;
	}
}

void BoardGameCanvas::SetTurnUIContext(TurnState _turnState, std::weak_ptr<UIContext> _context) {
	int index = (int)_turnState;
	if (contexts.size() < index)
		contexts.resize(index);

	contexts[index] = _context;
}
