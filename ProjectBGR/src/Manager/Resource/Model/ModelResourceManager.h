/*
 *	@brief モデルのハンドル管理
 *  @author Sekino
 */

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <unordered_map>
#include <string>

const char* const MODEL_FILEPATH = "res/Model/";	// オーディオのファイルパス
const char* const MODELDATA_FILEPATH = "res/ExternalFile/Resource/ModelData.json";	// オーディオデータのファイルパス

class ModelResourceManager {
private:
	// モデルハンドル管理配列
	std::unordered_map<std::string, int> models;

public:
	ModelResourceManager();
	~ModelResourceManager() = default;

	/*
	 * @brief モデルロード
	 */
	void LoadModel(const std::string& _name, const std::string& _path);

	/*
	 * @brief 管理しているハンドルを複製したものの取得
	 */
	int GetDupModelHandle(const std::string& _name);

	/*
	 * @brief 外部ファイルからの読み込み
	 */
	void LoadModelFromExternalFile();

	/*
	 * @brief 全削除
	 */
	void Clear();

};


#endif // !_MODELMANAGER_H_
