/*
 * @brief json繧剃ｽｿ縺・ｄ縺吶￥縺ｾ縺ｨ繧√◆繧ゅ・
 * @author Sekino
 */
#pragma once
#ifndef _MYJSON_H_
#define _MYJSON_H_

#include "ExternalLibrary/Json/json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

class MyJson {
public:

	/*
	 * @brief json繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ・医お繝ｳ繧ｳ繝ｼ繝・ぅ繝ｳ繧ｰ繧定・蜍募愛螳壹＠縺ｦ UTF-8 縺ｫ螟画鋤・・
	 */
	static json LoadJsonFile(const std::string& path);

    /*
	 * @brief UTF-8 縺ｮ繝舌う繝亥・繧・UTF-16 (std::wstring) 縺ｫ螟画鋤
	 */ 
	static std::wstring Utf8ToWString(const std::string& utf8);

	/*
	 * @brief UTF-8 縺ｮ繝舌う繝亥・繧堤樟蝨ｨ縺ｮ ANSI 繧ｳ繝ｼ繝峨・繝ｼ繧ｸ縺ｮ std::string 縺ｫ螟画鋤
	 */
	static std::string Utf8ToString(const std::string& utf8);

	/*
	 * @brief 謖・ｮ壹・髫主ｱ､莉･荳九↓縺ゅｋ繝輔ぃ繧､繝ｫ縺ｮ繝代せ縺ｨ蜷榊燕繧谷son縺ｫ縺ｾ縺ｨ繧√ｋ
	 */
	static bool CollectFilesToJson(const fs::path& rootDir, const fs::path& outputJsonPath, std::string jsonFileName);

	/*
	 * @brief jsonData繧偵ヰ繧､繝翫Μ(MessagePack)縺ｫ螟画鋤縺励｛utputPath縺ｸ譖ｸ縺榊・縺・
	 */
	static bool CompileBinary(const json& jsonData, const std::string& outputPath);

	/*
	 * @brief inputPath縺九ｉMessagePack繝舌う繝翫Μ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲）son繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺励※霑斐☆
	 */
	static json LoadBinary(const std::string& inputPath);
private:
	/*
	 * @breif 繝舌う繝医ヰ繝・ヵ繧｡繧・UTF-8 縺ｮ std::string 縺ｫ螟画鋤縺吶ｋ
	 */
	static std::string BufferToUtf8String(const std::vector<unsigned char>& buf);
};
#endif