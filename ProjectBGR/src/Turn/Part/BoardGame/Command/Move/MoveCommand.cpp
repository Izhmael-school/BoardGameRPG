#include "MoveCommand.h"
#include "Manager/Map/MapManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"
#include "Manager/Time/TimeManager.h"
#include "Definition/CommonModule/Json/MyJson.h"

#include <string>

bool MoveCommand::Move() {
	if (!currentPlayerData) return true;

	// 進める回数を超えていたら移動できない
	if (currentMoveCount >= canMoveCount) return true;

	InputManager& inputManager = InputManager::GetInstance();
	int port = currentPlayerData->GetControllerNum();

	// 進めるマスを調べる
	SearchCanMoveTiles();

	if (canMoveTileList.empty()) return true;

	bool ismoved = false;
	Vector2 movedDir = Vector2(0, 0);
	Vector2 beforeMoveTile = Vector2(-1, -1);
	Vector2 movedir = Vector2(0, 0);

	if (canMoveTileList[Move_Down] && inputManager.IsButtonDown(XINPUT_DPAD_DOWN)) {
		movedir = Vector2(0, 1);
		ismoved = true;
	}
	if (canMoveTileList[Move_Up] && inputManager.IsButtonDown(XINPUT_DPAD_UP)) {
		movedir = Vector2(0, -1);
		ismoved = true;
	}
	if (canMoveTileList[Move_Left] && inputManager.IsButtonDown(XINPUT_DPAD_LEFT)) {
		movedir = Vector2(-1, 0);
		ismoved = true;
	}
	if (canMoveTileList[Move_Right] && inputManager.IsButtonDown(XINPUT_DPAD_RIGHT)) {
		movedir = Vector2(1, 0);
		ismoved = true;
	}

	if (!ismoved) return false;

	beforeMoveTile = currentPlayerData->GetMapPosition();
	currentPlayerData->AddMapPosition(movedir);
	ismoved = true;
	movedDir = movedir;

	// 進んだ先のタイルIDが道だった場合は同じ方向にもう一度進む
	bool isMovedAgain = false;
	do {
		isMovedAgain = false;
		Vector2 mapPos = currentPlayerData->GetMapPosition();
		int x = mapPos.x;
		int y = mapPos.y;
		if (mapManager->GetMapList()[x][y] == static_cast<int>(MapTileType::Tile_Road)) {
			// 次のマスが道かマスでなければ進めない
			if (mapManager->GetMapList()[x + movedir.x][y + movedir.y] == static_cast<int>(MapTileType::Tile_Wall)) {
				currentPlayerData->SetMapPosition(beforeMoveTile);
				return false;
			}

			isMovedAgain = true;
			currentPlayerData->AddMapPosition(movedDir);
		}
	} while (isMovedAgain);

	// 道を戻っていたら戻ったことにする
	Vector2 prevPos = Vector2(-1, -1);
	if (!movedTiles.empty())
		prevPos = movedTiles.back();
	Vector2 currentPos = currentPlayerData->GetMapPosition();
	// 戻っていない
	if (prevPos.x != currentPos.x || prevPos.y != currentPos.y) {
		//進んだマスを記録
		movedTiles.push_back(beforeMoveTile);
		// 進んだ回数を増やす
		currentMoveCount++;
	}
	// 戻っている
	else {
		Back();
	}

	// 進める回数を超えたらターン終了
	if (currentMoveCount >= canMoveCount) {
		return true;
	}

	return false;
}

void MoveCommand::Back() {
	if (movedTiles.empty()) return;

	Vector2 back = movedTiles.back();
	movedTiles.pop_back();

	currentPlayerData->SetMapPosition(back);
	currentMoveCount--;
}

void MoveCommand::SearchCanMoveTiles() {
	if (!currentPlayerData || !mapManager) return;

	Vector2 mapPos = currentPlayerData->GetMapPosition();
	int x = mapPos.x;
	int y = mapPos.y;

	const MapTileArray& mapList = mapManager->GetMapList();

	std::vector<bool> canMoveList;

	// 上
	if (y > 0 && mapList[x][y - 1] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 下
	if (y < static_cast<int>(mapList[y].size()) - 1 && mapList[x][y + 1] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 左
	if (x > 0 && mapList[x - 1][y] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	// 右
	if (x < static_cast<int>(mapList.size()) - 1 && mapList[x + 1][y] != static_cast<int>(MapTileType::Tile_Wall)) {
		canMoveList.push_back(true);
	}
	else {
		canMoveList.push_back(false);
	}

	canMoveTileList = canMoveList;
}


bool MoveCommand::SpinRoulette() {
	static bool isStart = false;

	rouletteNum = currentPlayerData->GetRouletteNum();

	// ルーレットを作る
	int size = rouletteArray.size();
	if (size < rouletteNum) {
		for (int i = 0; i < rouletteNum - size; i++) {
			auto roulette = std::make_unique<Roulette>();
			roulette->RouletteStart(currentPlayerData->GetControllerNum());
			rouletteArray.emplace_back(std::move(roulette));
		}
	}

	// ルーレット更新
	float t = TimeManager::GetInstance().GetDeltaTime();
	int stopCount = 0;
	randCount = 0;
	for (auto& r : rouletteArray) {
		if (!isStart)
			r->RouletteStart(currentPlayerData->GetControllerNum());
		r->Update(t);
		bool isStop = r->IsStop();
		if (!isStop)continue;

		stopCount++;
		randCount += r->GetRouletteNum(7);
	}

	isStart = true;

	// ルーレット停止処理 
	if (stopCount >= rouletteNum) {
		isRouletteStop = true;
		canMoveCount = randCount;
		isStart = false;
		return true;
	}

	return false;
}

bool MoveCommand::ConfirmationMove() {
	// 進める回数まで進み切っていなければ戻る
	if (canMoveCount - currentMoveCount != 0) return dialogTextDelete = false;

	// 戻る
	if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_UP))
		confirmationMove = 0;

	// 止まる
	if (InputManager::GetInstance().IsButtonDown(XINPUT_DPAD_DOWN))
		confirmationMove = 1;

	if (InputManager::GetInstance().IsButtonDown(XINPUT_A)) {
		if (confirmationMove == 1)
			return dialogTextDelete = true;
		else {
			Back();
			commandState = MoveCommand_Move;
			dialogTextDelete = true;
		}
	}

	return false;
}

int MoveCommand::GetRouletteIndex(float _angle) {

	constexpr int CELL_COUNT = 7;

	constexpr float CELL_ANGLE = 360.0f / CELL_COUNT;

	_angle = std::fmod(_angle, 360.0f);
	if (_angle < 0.0f)
		_angle += 360.0f;

	int index = static_cast<int>(std::floor(_angle / CELL_ANGLE));

	return index % CELL_COUNT;
}

MoveCommandState MoveCommand::Execute() {
	// ルーレットを回してなければ戻れる
	if (isBack && InputManager::GetInstance().IsButtonDown(currentPlayerData->GetControllerNum(), XINPUT_B))
		return commandState = MoveCommand_Back;

	// ルーレットを回す
	if (!isRouletteStop)
		SpinRoulette();

	// 確定したらコマンド選択に戻れないようにする
	if (isBack && isRouletteStop) {
		isBack = false;
		Vector2 pos = currentPlayerData->GetMapPosition();
		mapManager->GetCanMoveTile(pos.x, pos.y, canMoveCount);
		return	commandState = MoveCommand_Move;
	}

	if (commandState == MoveCommand_RouletteTime)
		return commandState = MoveCommand_RouletteTime;

	// 移動
	if (Move()) {
		commandState = MoveCommand_Confirmation;
	}

	// 移動確定の最終確認
	if (ConfirmationMove())
		return commandState = MoveCommand_MoveEnd;

	return commandState;
}

void MoveCommand::Setup() {
	isBack = true;
	canMoveCount = -1;
	currentMoveCount = 0;
	movedTiles.clear();
	commandState = MoveCommand_RouletteTime;
	isRouletteStop = false;
	dialogTextDelete = false;
	ResetRouletteNum();
}

void MoveCommand::Render() {
	//VECTOR offset = VGet(400, 200, 0);
	//VECTOR offset2 = VGet(440, 240, 0);
	//int stringSize = 40;

	//// ウィンドウ
	//DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	//DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);

	//int extend = 2.0f;
	//int sx;
	//int sy;
	//GetDrawExtendStringSize(&sx, &sy, 0, extend * 2, extend, "1", 1);
	//int x = (offset2.x - (offset2.x - offset.x) / 2) - sx / 2;
	//int y = (offset2.y - (offset2.y - offset.y) / 2) - sy / 2;

	//int correctionX = stringSize - sx;
	//int correctionY = stringSize - sy;

	//DrawExtendFormatString(x + correctionX, y + correctionY, extend * 2, extend, 0xffff00, "%d", isBack ? randCount : canMoveCount - currentMoveCount);

	//// 移動を確定するかのウィンドウ
	//if (commandState != MoveCommand_Confirmation) return;

	//stringSize = 18;
	//offset = VGet(400, 240, 0);
	//offset2 = VGet(454, 276, 0);

	//// ウィンドウ
	//DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	//DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);
	//std::string back = "戻る";
	//std::string stop = "止まる";
	//DrawString(offset.x, offset.y, back.c_str(), confirmationMove == 0 ? 0xffff00 : 0xffffff);
	//DrawString(offset.x, offset.y + stringSize, stop.c_str(), confirmationMove == 1 ? 0xffff00 : 0xffffff);
}

void MoveCommand::FixedMove(int _moveNum) {
	// ルーレットを回したことにする
	isBack = false;
	isRouletteStop = true;
	canMoveCount = _moveNum;
	Vector2 pos = currentPlayerData->GetMapPosition();
	mapManager->GetCanMoveTile(pos.x, pos.y, canMoveCount);
	commandState = MoveCommand_Move;
}

std::vector<float> MoveCommand::GetRoletteAngle() const {
	std::vector<float> angleArray;

	for (auto& r : rouletteArray) {
		angleArray.emplace_back(r->GetAngle());
	}

	return angleArray;
}
