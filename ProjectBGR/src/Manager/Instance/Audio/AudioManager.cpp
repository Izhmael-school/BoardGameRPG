#include "AudioManager.h"
#include "Manager/Resource/Audio/AudioResourceManager.h"
#include "Instance/AudioInstance.h"

AudioManager::AudioManager(AudioResourceManager& _resourceManager)
	:pAudioResourceManager(_resourceManager)
	, instances() {
}

AudioPtr AudioManager::Play(const std::string& _audioName, float _volume, bool _isLoop, const Vector3& _pos, float _distance) {
	// 繝ｪ繧ｽ繝ｼ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
	auto resource = pAudioResourceManager.GetResource(_audioName);
	// 繝ｪ繧ｽ繝ｼ繧ｹ縺檎┌縺代ｌ縺ｰ蟶ｰ繧・
	if (!resource) return nullptr;
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ逕滓・
	auto instance = std::make_shared<AudioInstance>(resource, _volume, _isLoop, _distance);
	// 蜀咲函螟ｱ謨励＠縺溘ｉ蟶ｰ繧・
	if (!instance->Play(_pos)) return nullptr;
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤ｮ｡逅・ｸ九↓霑ｽ蜉
	instances.push_back(instance);
	return instance;
}

void AudioManager::Update(float _t) {
	for (auto& instance : instances) {
		instance->Update(_t);
	}

	// 蜀咲函縺檎ｵゅｏ縺｣縺溘ｉ豸医☆
	std::erase_if(instances, [](AudioPtr _instance) {
		return _instance->IsAudioEnd();
	});
}

void AudioManager::Render() {
}

void AudioManager::Clean() {
	StopAll();
	instances.clear();
	instances.shrink_to_fit();
}

void AudioManager::StopAll() {
	for (auto& instance : instances) {
		instance->Stop();
	}
}
