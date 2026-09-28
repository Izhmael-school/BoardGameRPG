#include "BattlePart.h"
#include <algorithm>
#include "DxLib.h"
#include "Data/Player/PlayerData.h"
#include "Data/Enemy/EnemyData.h"
#include "Manager/Input/InputManager.h"
#include "GameObject/GameObject.h" 
#include "Manager/GameObject/GameObjectManager.h"
#include "SpecialPhase/LevelUp/LevelUpPhase.h"

void BattlePart::Update(float _t) {
	switch (currentState) {
	case BattleState_AttackOrder:
		AttackOrder();
		MovePhase(BattleState_SelectCommand);
		break;
	case BattleState_SelectCommand:
		SelectCommand();
		MovePhase(BattleState_Battle);
		break;
	case BattleState_Battle:
	{

		static bool isBattle = false;
		static BattleState nextState = BattleState_TurnEnd;
		// ダメージ処理死亡処理は一回のみ
		if (!isBattle) {
			nextState = Battle();
			isBattle = true;
		}

		// 降参した場合
		if (isSurrender) {
			canPhaseMove = false;
			currentState = BattleState_Result;
			isBattle = false;
		}

		if (MovePhase(nextState))
			isBattle = false;
	}
	break;
	case BattleState_TurnChange:
		TurnChange();
		currentState = BattleState_SelectCommand;
		break;
	case BattleState_Result:
	{
		static bool isResult = false;
		static BattleState nextState = BattleState_TurnEnd;
		// リザルト処理は一回のみ
		if (!isResult) {
			nextState = Result();
			isResult = true;
		}
		if (MovePhase(nextState))
			isResult = false;

	}
	break;
	case BattleState_LevelUp:
	{
		static bool isLevelUp = false;

		// レベルアップ処理は一回のみ
		if (!isLevelUp) {
			if (LevelUpPhase::LevelUp()) {
				isLevelUp = true;
			}
			canPhaseMove = true;
		}

		if (MovePhase(BattleState_TurnEnd))
			isLevelUp = false;
	}
	break;
	case BattleState_TurnEnd:
		BattleEnd();
		break;
	}
}

#include "Definition/CommonModule/String/MyString.h"
#include <format>
void BattlePart::Render() {

	VECTOR offset = VGet(192, 108, 0);
	VECTOR offset2 = VGet(1728, 972, 0);
	float xCenter = 1920 / 2;
	float yCenter = 1080 / 2;
	if (currentState != BattleState_LevelUp) {

		// ステータステキスト
		std::vector<std::string> statusText = { "{} ATK {}","{} DEF {}","{} MAG {}","{} SPD {}","{} LUK {}" };
		std::vector<int> player1Status = { players[0]->GetAtk(),players[0]->GetDef(), players[0]->GetMag(), players[0]->GetSpd(), players[0]->GetLuk() };
		std::vector<int> player2Status = { players[1]->GetAtk(),players[1]->GetDef(), players[1]->GetMag(), players[1]->GetSpd(), players[1]->GetLuk() };
		int index = -2;
		int charExtend = 2;
		int charSize = 18 * charExtend;
		for (auto t : statusText) {
			int x = xCenter;
			int y = yCenter + charSize * index;
			std::string s = std::vformat(t, std::make_format_args(player1Status[index + 2], player2Status[index + 2]));
			MyString::StringCenterPos(s.c_str(), -1, &x, &y, charExtend, charExtend);
			DrawExtendString(x, y, charExtend, charExtend, s.c_str(), 0xffffff);
			index++;
		}

		int y = 216;
		int y2 = y + 50;
		int p1x = 1920 * 0.15f;
		int p1x2 = 1920 * 0.45f;
		float p1Hp = (float)players[0]->GetCurrentHp() / (float)players[0]->GetMaxHp();
		int p2x = 1920 * 0.55f;
		int p2x2 = 1920 * 0.85f;
		float p2Hp = (float)players[1]->GetCurrentHp() / (float)players[1]->GetMaxHp();
		VECTOR player1HpGaugeOffset = VGet(p1x, y, 0);
		VECTOR player1HpGaugeOffset2 = VGet(p1x + (p1x2 - p1x) * p1Hp, y2, 0);
		VECTOR player2HpGaugeOffset = VGet(p2x, y, 0);
		VECTOR player2HpGaugeOffset2 = VGet(p2x + (p2x2 - p2x) * p2Hp, y2, 0);

		DrawFillBox(player1HpGaugeOffset.x, player1HpGaugeOffset.y, player1HpGaugeOffset2.x, player1HpGaugeOffset2.y, 0x00ff00);
		DrawLineBox(player1HpGaugeOffset.x, player1HpGaugeOffset.y, p1x2, player1HpGaugeOffset2.y, 0x000000);
		DrawFillBox(player2HpGaugeOffset.x, player2HpGaugeOffset.y, player2HpGaugeOffset2.x, player2HpGaugeOffset2.y, 0x00ff00);
		DrawLineBox(player2HpGaugeOffset.x, player2HpGaugeOffset.y, p2x2, player2HpGaugeOffset2.y, 0x000000);

		charSize = 18;
		// 名前の描画
		DrawString(player1HpGaugeOffset.x, player1HpGaugeOffset.y - charSize, players[0]->GetPlayerName().c_str(), 0xffffff);
		auto player2Name = players[1]->GetPlayerName().c_str();
		int x = MyString::StringRightPos(player2Name, -1, player2HpGaugeOffset2.x);
		DrawString(x, player1HpGaugeOffset.y - charSize, player2Name, 0xffffff);
	}

	switch (currentState) {
	case BattleState_AttackOrder:
		DrawString(offset.x, offset.y, "攻守決め", 0xffffff);
		{

			std::string p1s = (attackPlayer == 0 ? "攻撃" : "防御");
			std::string p2s = (attackPlayer == 1 ? "攻撃" : "防御");

			int p1X = ((offset2.x - offset.x) / 4) + offset.x;
			int pY = yCenter;
			int p2X = (((offset2.x - offset.x) / 4) * 3) + offset.x;
			int a;

			int charExtend = 3;
			MyString::StringCenterPos(p1s.c_str(), -1, &p1X, &pY, charExtend, charExtend);
			DrawExtendString(p1X, pY, charExtend, charExtend, p1s.c_str(), attackPlayer == 0 ? 0xff0000 : 0x0000ff);
			MyString::StringCenterPos(p2s.c_str(), -1, &p2X, &a, charExtend, charExtend);
			DrawExtendString(p2X, pY, charExtend, charExtend, p2s.c_str(), attackPlayer == 1 ? 0xff0000 : 0x0000ff);
		}
		break;
	case BattleState_SelectCommand:
		DrawString(offset.x, offset.y, "コマンド選択", 0xffffff);
		offense.Render(attackPlayer);
		defence.Render(attackPlayer ^ 1);
		break;
	case BattleState_Battle:
	{
		DrawString(offset.x, offset.y, "ダメージ判定", 0xffffff);
		int x = xCenter;
		int y = yCenter + 300;
		std::string s = std::format("{0}は{1}をした。\n{2}の{3}！\n{0}に{4}ダメージ！", players[attackPlayer ^ 1]->GetPlayerName(), defence.GetSelectCommandName(), players[attackPlayer]->GetPlayerName(), offense.GetSelectCommandName(), damage);
		MyString::StringCenterPos(s.c_str(), -1, &x, &y);
		DrawString(x, y, s.c_str(), 0xffffff);
	}
	break;
	case BattleState_TurnChange:
		DrawString(offset.x, offset.y, "攻守反転", 0xffffff);

		break;
	case BattleState_Result:
		DrawString(offset.x, offset.y, "結果", 0xffffff);

		break;
	case BattleState_LevelUp:
		LevelUpPhase::Render();
		break;
	case BattleState_TurnEnd:
		DrawString(offset.x, offset.y, "ターン終了", 0xffffff);

		break;
	}
}

void BattlePart::Init(std::function<void()> _turnEndFunc, GameObjectManager* _gameObjectManager) {
	turnEnd = _turnEndFunc;
	players.resize(2);
	gameObjectManager = _gameObjectManager;
}

void BattlePart::BattleStart(CharacterData* _p1, CharacterData* _p2) {
	players[chara1] = _p1;
	players[chara2] = _p2;
	players[chara1]->SetBattling(true);
	players[chara2]->SetBattling(true);
	currentState = BattleState_AttackOrder;
	isTurnChange = false;
	winPlayer = -1;
	canPhaseMove = false;
	attackPlayer = -1;
	isSurrender = false;

	// なかったら取得
	if (!stage)
		stage = gameObjectManager->Find("BattleStage");
	if (!camera)
		camera = gameObjectManager->Find("Camera");

	player1Object = gameObjectManager->Find(players[chara1]->GetPlayerName());
	player2Object = gameObjectManager->Find(players[chara2]->GetPlayerName());

	Vector3 stagePos = VZero;
	if (stage) {
		stage->SetActive(true);
		Vector3 pos1 = stage->GetFramePos("Player1Pos");
		Vector3 pos2 = stage->GetFramePos("Player2Pos");
		if (player1Object)
			player1Object->GetTransform()->SetPosition(pos1);
		if (player2Object)
			player2Object->GetTransform()->SetPosition(pos2);
		stagePos = stage->GetTransform()->GetPosition();
	}
	if (camera) {
		camera->GetTransform()->SetPosition(Vector3::VAdd(Vector3::VScale(VUp, 1000), stagePos));
		camera->GetTransform()->SetRotation(Vector3(90, 0, 0));
	}
}

void BattlePart::AttackOrder() {
	// 先攻が決まってなければ決める
	if (attackPlayer != -1) return;

	// 素早さを取得
	int spd1 = players[chara1]->GetSpd();
	int spd2 = players[chara2]->GetSpd();

	// 素早さの差から先攻後攻を決める
	float ratio = spd1 / spd2;
	float rand = 50.0f + 50.0f * log2f(ratio);
	// どれだけ早かろうが90%、遅かろうが10%で先攻をとれるようにする
	rand = std::clamp(static_cast<int>(rand), 10, 90);
	// 乱数よりrand変数が高ければ
	attackPlayer = rand >= GetRand(100) ? 0 : 1;
	// コマンドにプレイヤー情報を渡す
	offense.StartTurn(players[attackPlayer]);
	defence.StartTurn(players[attackPlayer ^ 1]);
	// コマンド選択に移る
	canPhaseMove = true;
}

void BattlePart::SelectCommand() {
	offense.SelectCommand();
	defence.SelectCommand();
	// 両者コマンドを選択したらバトルに移る
	if (offense.IsSelect() && defence.IsSelect())
		canPhaseMove = true;
}

BattleState BattlePart::Battle() {
	canPhaseMove = true;
	// ダメージの計算と適応
	ResolveAction(offense.GetSelectCommand(), defence.GetSelectCommand());

	// 降参しているか確認
	if (isSurrender)
		return BattleState_Result;

	// 死亡確認
	if (players[damagePlayer]->IsDead()) {
		// 勝ったプレイヤーを記録
		winPlayer = damagePlayer ^ 1;
		// 死んでいたらバトル終了
		return BattleState_Result;
	}
	else {
		// 死んでいなければ攻守交替
		// すでに攻守交替していればバトルを終了させる
		if (!isTurnChange)
			return BattleState_TurnChange;
		else
			return BattleState_Result;
	}
}

void BattlePart::TurnChange() {
	attackPlayer = attackPlayer ^ 1;
	isTurnChange = true;
	// 選択をリセット
	offense.Reset();
	defence.Reset();
	offense.StartTurn(players[attackPlayer]);
	defence.StartTurn(players[attackPlayer ^ 1]);
}

BattleState BattlePart::Result() {
	canPhaseMove = true;
	// 勝者がいるか
	if (winPlayer == -1) {
		return BattleState_TurnEnd;
	}

	// いたら経験値やアイテムなど
	CharacterData* loser = players[winPlayer ^ 1];
	CharacterData* winner = players[winPlayer];
	PlayerData* pLoser = dynamic_cast<PlayerData*>(loser);
	PlayerData* pWinner = dynamic_cast<PlayerData*>(winner);
	EnemyData* eLoser = dynamic_cast<EnemyData*>(loser);
	EnemyData* eWinner = dynamic_cast<EnemyData*>(winner);

	// プレイヤー同士の戦いなら
	if (pLoser && pWinner && !isSurrender) {
		// 敗者がためてる経験値とレベルアップに必要な経験値の三割がもらえる
		int loserCurrentExp = loser->GetCurrentExp();
		winner->AddCurrentExp(pWinner->GetLevelUpNeedExp() * 0.3f + loserCurrentExp * 1.2f);
		// 経験値を半分減らす
		loser->SetCurrentExp(loserCurrentExp * 0.5f);
		// 所持金も半分渡す
		int loserMoney = loser->GetMoney() * 0.5f;
		winner->AddMoney(loserMoney);
		loser->SetMoney(loserMoney);
	}
	// 勝者がプレイヤーで敗者が敵モブ
	else if (pWinner && eLoser) {
		winner->AddCurrentExp(eLoser->GetCurrentExp() + 10000);
		winner->AddMoney(eLoser->GetMoney());
	}
	// 敗者のプレイヤーは初期地点に戻って１～３ターンランダムで動けなくなる
	if (pLoser && !isSurrender) {
		pLoser->SetDontMoveTurnNum(MyMath::Random(1, 3));
		pLoser->SetMapPosition(Vector3(3, 9));
		pLoser->Heal(10000000);
	}

	// 降参時に敵モブであれば敵データインスタンスを消す
	if (isSurrender && eWinner && !eWinner->IsBoss())
		eWinner->SetDelete(true);

	// 負けたのが敵なら敵データインスタンスを消す
	if (eLoser)
		eLoser->SetDelete(true);

	// 勝者がプレイヤーならレベルアップ処理に入る
	if (pWinner && pWinner->LevelUp()) {
		LevelUpPhase::SetLevelUpPlayer(pWinner);
		return BattleState_LevelUp;
	}

	return BattleState_TurnEnd;
}



void BattlePart::BattleEnd() {
	players[chara1]->SetBattling(false);
	players[chara2]->SetBattling(false);

	// ターン終了処理
	turnEnd();
}

void BattlePart::ResolveAction(OffenseSelectCommand _oc, DefenseSelectCommand _dc) {
	CharacterData* atkP = players[attackPlayer];
	CharacterData* defP = players[attackPlayer ^ 1];

	// 特殊な挙動するアクションを先に消化する
	switch (_dc) {
	case DefenseCommand_Counter:
		// カウンターは必殺技だけ反応する
		if (_oc != OffenseCommand_FatalAttack) break;
		{
			// カウンターは固定値プラス攻撃力の6倍
			damage = 1400 + defP->GetAtk() * 6;
			// ダメージを与える
			AddDamage(damage, atkP);
		}
		// ダメージを食らったほうを記録
		damagePlayer = attackPlayer;
		return;
	case DefenseCommand_Surrender:
		// 降参
		Surrender();
		return;
	}

	// 通常の処理
	bool isGuard = false;

	switch (_oc) {
	case OffenseCommand_Attack:
		// ガードならダメージを減らす
		if (_dc == DefenseCommand_Guard)
			isGuard = true;

		damage = CalcDamage(atkP->GetAtk(), defP->GetDef(), isGuard);
		break;
	case OffenseCommand_Magic:
		// 魔法ガードならダメージを減らす
		if (_dc == DefenseCommand_MagicGuard)
			isGuard = true;

		damage = CalcDamage(atkP->GetAtk(), defP->GetDef(), isGuard);
		break;
	case OffenseCommand_FatalAttack:
		// ガードならダメージを減らす
		if (_dc == DefenseCommand_Guard)
			isGuard = true;

		damage = CalcDamage(atkP->GetAtk(), defP->GetDef(), isGuard);
		break;
	}
	// ダメージを与える
	AddDamage(damage, defP);
	// ダメージを食らったほうを記録
	damagePlayer = attackPlayer ^ 1;
}

void BattlePart::Surrender() {
	winPlayer = attackPlayer;
	isSurrender = true;
	// 降参した側は1ターン動けなくする
	PlayerData* pd = dynamic_cast<PlayerData*>(players[attackPlayer ^ 1]);
	if (pd)
		pd->SetDontMoveTurnNum(1);
}

int BattlePart::CalcDamage(int _atk, int _def, bool isGuard) {
	long long dmg = ((long long)_atk * 7 / 3 - _def) * 5 / 2;
	// ダメージを一律35%減
	if (isGuard) dmg = dmg * 65 / 100;
	return (int)max(1LL, dmg);
}

void BattlePart::AddDamage(int _dmg, CharacterData* _target) { _target->Damage(_dmg); }

bool BattlePart::MovePhase(BattleState _nextState) {
	if (canPhaseMove && InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN)) {
		canPhaseMove = false;
		currentState = _nextState;
		return true;
	}

	return false;
}
