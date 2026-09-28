#pragma once
#include "../CharacterData.h"
class EnemyData : public CharacterData {
private:
	bool isBoss = false;
	bool isDelete = false;

public:
	~EnemyData() override;

	void SetBoss(bool _isBoss) { isBoss = _isBoss; }
	bool IsBoss() const { return isBoss; }

	void SetDelete(bool _isDelete) { isDelete = _isDelete; }
	bool IsDelete() const { return isDelete; }
};