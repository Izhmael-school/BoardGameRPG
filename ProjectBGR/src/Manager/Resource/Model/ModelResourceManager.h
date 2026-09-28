/*
 *	@brief 繝｢繝・Ν縺ｮ繝上Φ繝峨Ν邂｡逅・
 *  @author Sekino
 */

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <unordered_map>
#include <string>

const char* const MODEL_FILEPATH = "res/Model/";	// 繧ｪ繝ｼ繝・ぅ繧ｪ縺ｮ繝輔ぃ繧､繝ｫ繝代せ
const char* const MODELDATA_FILEPATH = "res/ExternalFile/Resource/ModelData.json";	// 繧ｪ繝ｼ繝・ぅ繧ｪ繝・・繧ｿ縺ｮ繝輔ぃ繧､繝ｫ繝代せ

class ModelResourceManager {
private:
	// 繝｢繝・Ν繝上Φ繝峨Ν邂｡逅・・蛻・
	std::unordered_map<std::string, int> models;

public:
	ModelResourceManager();
	~ModelResourceManager();

	/*
	 * @brief 繝｢繝・Ν繝ｭ繝ｼ繝・
	 */
	void LoadModel(const std::string& _name, const std::string& _path);

	/*
	 * @brief 邂｡逅・＠縺ｦ縺・ｋ繝上Φ繝峨Ν繧定､・｣ｽ縺励◆繧ゅ・縺ｮ蜿門ｾ・
	 */
	int GetDupModelHandle(const std::string& _name);

	/*
	 * @brief 螟夜Κ繝輔ぃ繧､繝ｫ縺九ｉ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	void LoadModelFromExternalFile();

	/*
	 * @brief 蜈ｨ蜑企勁
	 */
	void Clear();

	/*
	 * @brief 読み込んだモデルの数
	 */
	int GetModelNum() const { return models.size(); };
};


#endif // !_MODELMANAGER_H_
