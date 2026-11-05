/*
 *	@brief 繝｢繝・Ν縺ｮ繝上Φ繝峨Ν邂｡逅・
 *  @author Sekino
 */

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <unordered_map>
#include <string>

const char* const MODEL_FILEPATH = "res/Model/";	// モデルが格納されてるフォルダ階層

class ModelResourceManager {
private:
	// 繝｢繝・Ν繝上Φ繝峨Ν邂｡逅・・蛻・
	std::unordered_map<std::string, int> models;

public:
	ModelResourceManager();
	~ModelResourceManager();

	/*
	 * @brief モデルの読み込み
	 */
	void LoadModel(const std::string& _name, const std::string& _path);

	/*
	 * @brief 名前指定で複製したモデルハンドルを取得
	 */
	int GetDupModelHandle(const std::string& _name);

	/*
	 * @brief 指定のファイル階層以下にあるMV1ファイルをすべて読み込む
	 */
	void LoadAllModel();

	/*
	 * @brief 全削除
	 */
	void Clear();

	/*
	 * @brief 読み込んだモデルの数
	 */
	int GetModelNum() const { return models.size(); };
};


#endif // !_MODELMANAGER_H_
