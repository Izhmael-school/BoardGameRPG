/*
 * @brief 繧｢繧､繝・Β繧呈桶縺・さ繝槭Φ繝・
 * @author Sekino
 */

#pragma once
#ifndef _ITEMCOMMAND_H_
#define _ITEMCOMMAND_H_

class PlayerData;

class ItemCommand {
private:
	int selectItemIndex;
	PlayerData* currentSelectPlayer;
public:
	/*
	 * @brief 繧｢繧､繝・Β繧帝∈謚槭☆繧・
	 * @return 驕ｸ謚槭＠縺溷ｴ蜷・true縲√◎繧御ｻ･螟悶・ false
	 */
	bool SelectItem();

	/*
	 * @brief 繧｢繧､繝・Β繧剃ｽｿ縺・
	 */
	void UseItem(int _effectID);

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

	int GetSelectItemIndex() const { return selectItemIndex; }
};

#endif