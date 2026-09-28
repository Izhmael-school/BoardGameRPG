#include "AudioInstance.h"
#include "DxLib.h"
#include "Manager/Resource/ResourceBase.h"
#include "Library/Vector/ConversionVECTOR.h"

AudioInstance::AudioInstance(AudioResourcePtr _audioResource, float _volume, bool _isLoop, float _distance)
	:InstanceBase(_audioResource)
	, playHandle(-1)
	, volume(_volume)
	, distance(_distance)
	, isLoop(_isLoop)
	, is3D(_audioResource->Is3D()) {
}

AudioInstance::~AudioInstance() {
	if (playHandle != -1)
		DeleteSoundMem(playHandle);
}

void AudioInstance::Update(float _t) {
	GameObject::Update(_t);

	// 髻ｳ驥上・險ｭ螳・
	ChangeVolumeSoundMem(static_cast<int>(volume), playHandle);

	// 3D髻ｳ貅舌・險ｭ螳・
	if (!is3D) return;

	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(GetTransform()->GetPosition());
	// 3D髻ｳ貅舌・菴咲ｽｮ繧定ｨｭ螳・
	Set3DPositionSoundMem(pos, playHandle);
	// 3D髻ｳ貅舌・霍晞屬繧定ｨｭ螳・
	Set3DRadiusSoundMem(distance, playHandle);
}

void AudioInstance::Render() {
}

bool AudioInstance::Play(Vector3 _pos) {
	// 莠碁㍾蜀咲函遖∵ｭ｢
	if (!IsAudioEnd()) return false;
	// 繧ｵ繧ｦ繝ｳ繝峨・隍・｣ｽ
	playHandle = DuplicateSoundMem(resource->GetHandle());

	// 繝ｫ繝ｼ繝苓ｨｭ螳・
	int playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
	// 蜀咲函
	PlaySoundMem(playHandle, playType);

	if (!is3D) return true;

	// 3D髻ｳ貅舌・菴咲ｽｮ繧定ｨｭ螳・
	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(_pos);
	Set3DPositionSoundMem(pos, playHandle);
	// 3D髻ｳ貅舌・霍晞屬繧定ｨｭ螳・
	Set3DRadiusSoundMem(distance, playHandle);

	return true;
}

void AudioInstance::Stop() {
	// 蜀咲函縺励※辟｡縺代ｌ縺ｰ蟶ｰ繧・
	if (IsAudioEnd()) return;
	// 蛛懈ｭ｢
	StopSoundMem(playHandle);
	playHandle = -1;
}

const bool AudioInstance::IsAudioEnd() const {
	// 蜀咲函縺励※縺・↑縺代ｌ縺ｰ邨ゆｺ・
	if (playHandle == -1) return true;
	return CheckSoundMem(playHandle) == 0;
}

void AudioInstance::SetClassID() {
	classID = typeid(this);
}
