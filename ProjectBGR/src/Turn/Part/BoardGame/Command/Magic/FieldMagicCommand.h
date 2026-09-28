/*
 * @brief 繝輔ぅ繝ｼ繝ｫ繝我ｸ翫〒縺ｮ鬲疲ｳ輔ｒ謇ｱ縺・さ繝槭Φ繝・
 * @author Sekino
 */

#pragma once
#ifndef _MagicCOMMAND_H_
#define _MagicCOMMAND_H_

class PlayerData;

class FieldMagicCommand {
private:
	int selectMagicIndex;
	PlayerData* currentSelectPlayer;
public:
	/*
	 * @brief 鬲疲ｳ輔ｒ驕ｸ謚槭☆繧・
	 * @return 驕ｸ謚槭＠縺溷ｴ蜷・true縲√◎繧御ｻ･螟悶・ false
	 */
	bool SelectMagic();

	/*
	 * @brief 鬲疲ｳ輔ｒ菴ｿ縺・
	 */
	void UseMagic(int _effectID);

	/*
	 * @brief 謠冗判
	 */
	void Render();

	/*
	 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ髢句ｧ区凾縺ｫ迴ｾ蝨ｨ縺ｮ繝励Ξ繧､繝､繝ｼ繧呈ｸ｡縺励※繧ゅｉ縺・
	 */
	void SetCurrentTurnPlayer(PlayerData* _playerData) { currentSelectPlayer = _playerData; };

	/*
	 * @brief 繧ｳ繝槭Φ繝蛾∈謚樒判髱｢縺ｫ謌ｻ繧句燕縺ｫ蛻晄悄蛹・
	 */
	bool Return();

	int GetSelectMagicIndex() const { return selectMagicIndex; }
};

#endif