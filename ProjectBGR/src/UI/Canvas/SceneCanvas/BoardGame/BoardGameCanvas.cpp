#include "BoardGameCanvas.h"
#include "UI/UIObject/UIText/UIText.h"
#include "UI/UIObject/UIText/UITextDialog/UITextDialog.h"
#include "UI/UIObject/UIButton/UIButton.h"
#include "UI/UIObject/UISprite/UISprite.h"
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
	info.itemList = p->GetItemList();
	info.magicList = p->GetMagicList();
	info.equipList = p->GetEquipList();
}

void BoardGameCanvas::OpenItemListUI() {
	if (itemListUI) return;

	Vector2 offset = Vector2(400, 200);
	Vector2 offset2 = Vector2(700, 400);
	itemListUI = Instantiate<UISprite>(-1, offset, offset2.x - offset.x, offset2.y - offset.y);
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle itemUIStyle;
	itemUIStyle.normalColor = 0x000000;
	itemUIStyle.fontSize = 20;
	for (auto& item : info.itemList) {
		ItemData itemData = ElementDataManager::GetInstance().GetItemData(item);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int x = (offset.x + (itemUIStyle.fontSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + itemUIStyle.fontSize) + (itemUIStyle.fontSize * (i / 2));

		Vector2 pPos = itemListUI->GetPosition();

		itemListUI->AddChild(UniqueInstantiate<UIText>(itemData.name, itemUIStyle, Vector2(x - pPos.x, y - pPos.y)));
		i++;
	}
}

void BoardGameCanvas::CloseItemListUI() {
	if (!itemListUI) return;

	// 生ポインタとコンテナ要素を比較するため find_if を使う
	auto itr = std::find_if(uiArray.begin(), uiArray.end(),
		[&](const auto& p) { return p.get() == itemListUI; });

	if (itr == uiArray.end()) return;

	// 所有権を解放してからコンテナから削除
	(*itr).reset();
	uiArray.erase(itr);
	itemListUI = nullptr;
}

void BoardGameCanvas::UpdateItemListUI() {
	if (!itemListUI)
		OpenItemListUI();

	auto children = itemListUI->GetChildren();
	int i = 0;
	for (auto& child : children) {
		UIText* text = static_cast<UIText*>(child);

		if (i != selectIndex)
			text->SetSelect(false);
		else {
			text->SetSelect(true);
			std::string itemExplanation = ElementDataManager::GetInstance().GetItemData(info.itemList[i]).explanation;
			explanationUI->SetText(itemExplanation);
		}

		i++;
	}

}

void BoardGameCanvas::OpenMagicListUI() {
	if (magicListUI) return;

	Vector2 offset = Vector2(400, 200);
	Vector2 offset2 = Vector2(700, 400);
	magicListUI = Instantiate<UISprite>(-1, offset, offset2.x - offset.x, offset2.y - offset.y);
	int i = 0;
	unsigned int color = 0xffffff;
	UITextStyle magicUIStyle;
	magicUIStyle.normalColor = 0x000000;
	magicUIStyle.fontSize = 20;
	for (auto& magic : info.magicList) {
		MagicData magicData = ElementDataManager::GetInstance().GetMagicData(magic);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		int x = (offset.x + (magicUIStyle.fontSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + magicUIStyle.fontSize) + (magicUIStyle.fontSize * (i / 2));

		Vector2 pPos = magicListUI->GetPosition();

		magicListUI->AddChild(UniqueInstantiate<UIText>(magicData.name, magicUIStyle, Vector2(x - pPos.x, y - pPos.y)));
		i++;
	}
}

void BoardGameCanvas::CloseMagicListUI() {
	if (!magicListUI) return;

	// 生ポインタとコンテナ要素を比較するため find_if を使う
	auto itr = std::find_if(uiArray.begin(), uiArray.end(),
		[&](const auto& p) { return p.get() == magicListUI; });

	if (itr == uiArray.end()) return;

	// 所有権を解放してからコンテナから削除
	(*itr).reset();
	uiArray.erase(itr);
	magicListUI = nullptr;
}

void BoardGameCanvas::UpdateMagicListUI() {
	if (!magicListUI)
		OpenMagicListUI();

	auto children = magicListUI->GetChildren();
	int i = 0;
	for (auto& child : children) {
		UIText* text = static_cast<UIText*>(child);

		if (i != selectIndex)
			text->SetSelect(false);
		else {
			text->SetSelect(true);
			std::string magicExplanation = ElementDataManager::GetInstance().GetMagicData(info.magicList[i]).explanation;
			explanationUI->SetText(magicExplanation);
		}

		i++;
	}

}

void BoardGameCanvas::OpenMoveCommandUI() {
	if (moveCommandUI) return;

	moveCommandUI = Instantiate<UIObject>();

	auto spriteManager = Application::GetInstance().GetSpriteResourceManager();
	int rouletteHandle = spriteManager->GetSprite("Roulette");
	int arrowHandle = spriteManager->GetSprite("RouletteArrow");

	int w = 1920;
	int h = 1080;
	// ルーレットの生成
	auto roulette = UniqueInstantiate<UIRoulette>(rouletteHandle, arrowHandle, Vector2(w / 2, (h / 3) * 2), 1, 1, "Roulette");
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

	auto rouletteInfo = std::get_if<MoveCommandUINeedInfo>(&binfo.info);

	if (!rouletteInfo) return;

	auto roulette = static_cast<UIRoulette*>(moveCommandUI->GetChild("Roulette"));
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
			roulette->SetActive(true);
			roulette->SetArrowAngle(rouletteInfo->angle);
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

	commandSelectUI = Instantiate<UIObject>();

	// Dialogの設定
	UITextStyle explanation;
	explanation.normalColor = 0xffffff;
	explanation.fontSize = 50;
	auto dialog = UniqueInstantiate<UITextDialog>(explanation, Vector2(250, 700), -1, "Dialog");
	commandSelectUI->AddChild(std::move(dialog));

	auto options = UniqueInstantiate<UIObject>("Options");
	auto& commandList = std::get_if<CommandSelectUINeedInfo>(&binfo.info)->commandList;

	// コマンドの設定
	int index = 0;
	for (auto& command : commandList) {
		int x = 250;
		int y = 200 + explanation.fontSize * index;
		auto option = UniqueInstantiate<UIText>(command.name, explanation, Vector2(x, y));
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

	// ダイアログにテキストを送ったか
	static bool isSendText = false;
	// １フレーム前に選択していたコマンドの値
	static int prevSelectIndex = -1;
	// 今選択しているコマンドの値
	int currentSelectIndex = commandSelectInfo->selectIndex;

	auto dialogUI = static_cast<UITextDialog*>(commandSelectUI->GetChild("Dialog"));
	if (currentSelectIndex != prevSelectIndex) {
		isSendText = false;
		if (dialogUI)
			dialogUI->DeleteFrontText();
	}

	for (int i = 0, max = children.size(); i < max; ++i) {
		auto text = static_cast<UIText*>(children[i]);

		bool isSelected = (currentSelectIndex == i);

		text->SetSelect(isSelected);

		if (!isSelected) continue;

		// 選択されているコマンドの説明を表示

		if (!dialogUI || isSendText) return;

		dialogUI->SetActive(true);

		dialogUI->AddText(commandSelectInfo->commandList[i].explanation, []() { return  false; });

		isSendText = true;
	}

	prevSelectIndex = currentSelectIndex;
}

void BoardGameCanvas::CloseCommandSelectUI() {
	DeleteUI(commandSelectUI);
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

	if(dialogUI && !isDialogTextSend) {
		dialogUI->SetActive(true);
		dialogUI->AddText(MyString::MergeString(tileEffectInfo->elementNameList[tileEffectInfo->currentSelect] ,"を手に入れた！"), []() { return false; });
		isDialogTextSend = true;
	}
}

void BoardGameCanvas::CloseTileEffectUI() {
	DeleteUI(tileEffectUI);
	isDialogTextSend = false;
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
		break;
	default:
		break;
	}
}
