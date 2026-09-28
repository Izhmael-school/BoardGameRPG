/*
 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once

#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_

#include "../../ManagerBase.h"
#include <memory>
#include <string>
#include <vector>
#include "Library/Vector/Vector3.h"

class AudioInstance;
class AudioResourceManager;

using AudioPtr = std::shared_ptr<AudioInstance>;

class AudioManager : public ManagerBase {
private:
	std::vector<AudioPtr> instances;	// 邂｡逅・ｸ九↓縺ゅｋ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ
public:
	AudioResourceManager& pAudioResourceManager;// 隱ｭ縺ｿ霎ｼ縺ｿ逕ｨ

public:
	AudioManager(AudioResourceManager& _resourceManager);

	/*
	 * @brief 逕滓・
	 */
	AudioPtr Play(const std::string& _audioName, float _volume = 255.0f, bool _isLoop = false, const Vector3& _pos = VZero, float _distance = 10.0f);

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

	/*
	 * @brief 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ蜈ｨ蜑企勁
	 */
	void Clean();

	/*
	 * @brief 蜈ｨ縺ｦ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧貞●豁｢
	 */
	void StopAll();

	/*
	 * @brief 邂｡逅・＠縺ｦ繧九う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ謨ｰ
	 */
	int GetInstanceCount() const { return instances.size(); }

};
#endif // !_AUDIOMANAGER_H_