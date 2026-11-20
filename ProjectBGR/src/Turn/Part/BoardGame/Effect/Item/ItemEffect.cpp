#include "ItemEffect.h"
#include "Data/Player/PlayerData.h"

std::function<void(int)> ItemAndMagicEffect::fixedMoveFunc = nullptr;
std::function<Vector2(Vector2)> ItemAndMagicEffect::randomWarpFunc = nullptr;
std::function<void()> ItemAndMagicEffect::moveEndFunc = nullptr;
int ItemAndMagicEffect::usedItemAnyInfo = -1;

enum ItemEffectEnum {
	Effect_DoublingStatus,
	Effect_RouletteIncrease,
	Effect_HPHeal,
	Effect_FixedMove,
	Effect_Gamble,
	Effect_GoHome,
	Effect_RandomWarp,
	Effect_Damage,
	Effect_TargetRandomWarp,
	Effect_ChangePosition,
	Effect_GoTargetPosition,
	Effect_Together,
	Effect_BreakItem
};

void ItemAndMagicEffect::EffectExecute(int effectID, int effectValue, int subEffectValue, CharacterData* _chara, std::vector<CharacterData*> _target) {
	if (!_chara) return;

	ItemEffectEnum i = static_cast<ItemEffectEnum>(effectID);

	switch (i) {
	case Effect_DoublingStatus:
		DoublingStatus(effectValue, _chara);
		break;
	case Effect_RouletteIncrease:
		RouletteIncrease(effectValue, _chara);
		break;
	case Effect_HPHeal:
		HPHeal(effectValue, _chara);
		break;
	case Effect_FixedMove:
		FixedMove(effectValue, _chara);
		break;
	case Effect_Gamble:
		Gamble(effectValue, subEffectValue, _chara);
		break;
	case Effect_GoHome:
		GoHome(_chara);
		break;
	case Effect_RandomWarp:
		RandomWarp(_chara);
		// 移動終了
		if (moveEndFunc)
			moveEndFunc();
		break;
	case Effect_Damage:
		Damage(effectValue,_chara,_target);
		break;
	case Effect_TargetRandomWarp:
		for (auto t : _target) {
			RandomWarp(t);
		}
		break;
	case Effect_Together:
		GoTargetPosition(_chara,_target.front() );
		break;
	case Effect_GoTargetPosition:
		GoTargetPosition(_target.front(), _chara);
		// 移動終了
		if (moveEndFunc)
			moveEndFunc();
		break;
	case Effect_ChangePosition:
		ChangePosition(_chara, _target.front());
		// 移動終了
		if (moveEndFunc)
			moveEndFunc();
		break;
	case Effect_BreakItem:
		BreakItem(_target.front());
		break;
	default:
		break;
	}
}

void ItemAndMagicEffect::RouletteIncrease(int _effectValue, CharacterData* _chara) {
	PlayerData* player = static_cast<PlayerData*>(_chara);

	if (!player) return;

	player->SetRouletteNum(_effectValue);
}

void ItemAndMagicEffect::HPHeal(int _effectValue, CharacterData* _chara) {
	PlayerData* player = static_cast<PlayerData*>(_chara);

	if (!player) return;

	int max = player->GetMaxHp();

	int healValue = max / _effectValue;

	usedItemAnyInfo = healValue;

	player->Heal(healValue);
}

void ItemAndMagicEffect::FixedMove(int _effectValue, CharacterData* _chara) {
	if (fixedMoveFunc)
		fixedMoveFunc(_effectValue);
}

#include "Definition/Enum/StatusChange.h"
void ItemAndMagicEffect::DoublingStatus(int _status,CharacterData* _chara) {
	// 3ターンステータスを変化
	_chara->GetStatus()->AddStatusChange(static_cast<StatusChange>(_status), 3);
}

void ItemAndMagicEffect::Gamble(int _prob, int _mag, CharacterData* _chara) {
	int rand = MyMath::Random(1, 100);

	int afterMoney = 0;
	if (rand <= _prob)
		afterMoney = _chara->GetMoney() * _mag;
	else
		afterMoney = 0;
	
	usedItemAnyInfo = afterMoney;
	_chara->SetMoney(afterMoney);
}

void ItemAndMagicEffect::GoHome(CharacterData* _chara) {
	Vector2 pos = _chara->GetRespawnPoint();
	_chara->SetMapPosition(pos);

	// 移動終了
	if (moveEndFunc)
		moveEndFunc();
}

void ItemAndMagicEffect::RandomWarp(CharacterData* _chara) {
	if (!randomWarpFunc) return;

	// 今いる座標以外のランダムな座標をもらう
	Vector2 randPos = randomWarpFunc(_chara->GetMapPosition());
	// ワープ
	_chara->SetMapPosition(randPos);
}

void ItemAndMagicEffect::Damage(int _effectValue, CharacterData* _chara, std::vector<CharacterData*> _target) {
	if (_target.size() == 0) return;
	usedItemAnyInfo = 0;
	for (auto& t : _target) {
		int dMag = t->GetMag();
		int aMag = _chara->GetMag() * _effectValue;

		int damage = ((long long)aMag * 7 / 3 - dMag) * 5 / 2;

		usedItemAnyInfo += damage;
		t->Damage(damage);
	}
}

void ItemAndMagicEffect::ChangePosition(CharacterData* _chara, CharacterData* _target) {
	Vector2 pos = _chara->GetMapPosition();
	_chara->SetMapPosition(_target->GetMapPosition());
	_target->SetMapPosition(pos);
}

void ItemAndMagicEffect::GoTargetPosition(CharacterData* _come, CharacterData* _go) {
	_go->SetMapPosition(_come->GetMapPosition());
}

void ItemAndMagicEffect::BreakItem(CharacterData* _target) {
	PlayerData* p = static_cast<PlayerData*>(_target);

	if (!p) return;

	int size = p->GetItemList().size();

	int rand = MyMath::Random(0, size - 1);

	p->RemoveItem(rand);
}
