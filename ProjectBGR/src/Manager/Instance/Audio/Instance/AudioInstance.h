/*
 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _AUDIOINSTANCE_H_
#define _AUDIOINSTANCE_H_

#include "../../InstanceBase.h"
#include "Manager/Resource/Audio/Resource/AudioResource.h"
#include "Library/Vector/Vector3.h"

class AudioInstance : public InstanceBase {
private:
	int playHandle;	// 繝ｪ繧ｽ繝ｼ繧ｹ縺梧戟縺｣縺ｦ縺・ｋ繝上Φ繝峨Ν
	float volume;	// 髻ｳ驥・
	float distance;	// 3D髻ｳ貅舌・霍晞屬
	bool isLoop;	// 繝ｫ繝ｼ繝怜・逕溘☆繧九°
	bool is3D;		// 3D髻ｳ貅舌°縺ｩ縺・°

public:
	AudioInstance(AudioResourcePtr _audioResource, float _volume = 255.0f, bool _isLoop = false, float _distance = 10.0f);
	~AudioInstance();

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

	/*
	 * @brief 蜀咲函
	 * @param _pos 蜀咲函縺吶ｋ蠎ｧ讓・is3D髻ｳ貅舌・蝣ｴ蜷医・縺ｿ菴ｿ逕ｨ)
	 */
	bool Play(Vector3 _pos = VZero);

	/*
	 * @brief 蛛懈ｭ｢
	 */
	void Stop();

	/*
	 * @brief 蜀咲函縺檎ｵゅｏ縺｣縺ｦ繧九°
	 */
	const bool IsAudioEnd() const;

private:
	void SetClassID();
};
#endif