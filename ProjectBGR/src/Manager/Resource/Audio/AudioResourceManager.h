/*
 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ縺ｮ繝ｪ繧ｽ繝ｼ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _AUDIORESOURCEMANAGER_H_
#define _AUDIORESOURCEMANAGER_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "Resource/AudioResource.h"

const char* const AUDIO_FILEPATH = "res/Audio/";	// 繧ｪ繝ｼ繝・ぅ繧ｪ縺ｮ繝輔ぃ繧､繝ｫ繝代せ
const char* const AUDIODATA_FILEPATH = "res/ExternalFile/Resource/AudioData.json";	// 繧ｪ繝ｼ繝・ぅ繧ｪ繝・・繧ｿ縺ｮ繝輔ぃ繧､繝ｫ繝代せ

class AudioResourceManager {
private:
	std::unordered_map<std::string, AudioResourcePtr> resources;	// 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜷榊燕縺ｨ繝上Φ繝峨Ν縺ｮ繝槭ャ繝・

public:
	AudioResourceManager();
	~AudioResourceManager() = default;

	/*
	 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	bool LoadAudio(const std::string& _name, const std::string& _path, bool _is3D = false);

	/*
	 * @brief 螟夜Κ繝輔ぃ繧､繝ｫ縺九ｉ縺ｮ繧ｪ繝ｼ繝・ぅ繧ｪ隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	void LoadAudioFromExternalFile();

	/*
	 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜿門ｾ・
	 */
	AudioResourcePtr GetResource(const std::string& _name) const;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ繧薙□繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ謨ｰ蜿門ｾ・
	 */
	int GetAudioResourceCount() const { return resources.size(); }

	/*
	 * @brief 蜈ｨ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜑企勁
	 */
	void Clear() { resources.clear(); }
};
#endif