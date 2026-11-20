/*
 * @brief 繝輔ぅ繝ｼ繝ｫ繝我ｸ翫〒縺ｮ鬲疲ｳ輔ｒ謇ｱ縺・さ繝槭Φ繝・
 * @author Sekino
 */

#pragma once
#ifndef _MagicCOMMAND_H_
#define _MagicCOMMAND_H_

#include <vector>
#include <memory>
#include <string>

class PlayerData;
class CharacterData;
class CharacterDataManager;
class UIContext;

class FieldMagicCommand {
private:
	bool isSelectMagic = true;
	bool isSelectTarget = false;
	bool isUsed = false;
	bool isConfirm = false;
	bool isCancel = false;
	int selectMagicIndex;
	PlayerData* currentTurnPlayer;
	CharacterDataManager* characterDataManager;
	std::vector<CharacterData*> possibleTargetArray;
	bool isGetPossibleTargets;
	int selectTargetIndex = 0;
	std::vector<CharacterData*> target;
	std::shared_ptr<UIContext> context;
	std::string magicName;

public:
	void Init();

	/*
	 * @brief 鬲疲ｳ輔ｒ驕ｸ謚槭☆繧・
	 * @return 驕ｸ謚槭＠縺溷ｴ蜷・true縲√◎繧御ｻ･螟悶・ false
	 */
	bool SelectMagic();

	/*
	 * @brief 鬲疲ｳ輔ｒ菴ｿ縺・
	 */
	void UseMagic();

	/*
	 * @brief 魔法を使う相手を決める
	 */
	bool SelectTarget();

	/*
	 * @brief 謠冗判
	 */
	void Render();

	/*
	 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ髢句ｧ区凾縺ｫ迴ｾ蝨ｨ縺ｮ繝励Ξ繧､繝､繝ｼ繧呈ｸ｡縺励※繧ゅｉ縺・
	 */
	void SetCurrentTurnPlayer(PlayerData* _playerData) { currentTurnPlayer = _playerData; };

	void SetCharacterDataManager(CharacterDataManager* _characterDataManager) { characterDataManager = _characterDataManager; }

	/*
	 * @brief 繧ｳ繝槭Φ繝蛾∈謚樒判髱｢縺ｫ謌ｻ繧句燕縺ｫ蛻晄悄蛹・
	 */
	bool Return();

	int GetSelectMagicIndex() const { return selectMagicIndex; }
	int GetSelectTargetIndex() const { return selectTargetIndex; }

	bool IsSelectItem() const { return isSelectMagic; }
	bool IsSelectTarget() const { return isSelectTarget; }

	std::shared_ptr<UIContext> GetUIContext() const { return context; }

	void RemoveUsedMagic();

	bool IsCancel() const { return isCancel; }

	void SetCancel(bool _isCancel) { isCancel = _isCancel; }

	bool IsUsed() const { return isUsed; }

	bool SetUsed(bool _isUsed) { isUsed = _isUsed; }
private:
	std::vector<CharacterData*> TargetSearch();
};

#endif