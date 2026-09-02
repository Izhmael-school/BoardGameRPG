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

	// 音量の設定
	ChangeVolumeSoundMem(static_cast<int>(volume), playHandle);

	// 3D音源の設定
	if (!is3D) return;

	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(GetTransform()->GetPosition());
	// 3D音源の位置を設定
	Set3DPositionSoundMem(pos, playHandle);
	// 3D音源の距離を設定
	Set3DRadiusSoundMem(distance, playHandle);
}

void AudioInstance::Render() {
}

bool AudioInstance::Play(Vector3 _pos) {
	// 二重再生禁止
	if (!IsAudioEnd()) return false;
	// サウンドの複製
	playHandle = DuplicateSoundMem(resource->GetHandle());

	// ループ設定
	int playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
	// 再生
	PlaySoundMem(playHandle, playType);

	if (!is3D) return true;

	// 3D音源の位置を設定
	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(_pos);
	Set3DPositionSoundMem(pos, playHandle);
	// 3D音源の距離を設定
	Set3DRadiusSoundMem(distance, playHandle);

	return true;
}

void AudioInstance::Stop() {
	// 再生して無ければ帰る
	if (IsAudioEnd()) return;
	// 停止
	StopSoundMem(playHandle);
	playHandle = -1;
}

const bool AudioInstance::IsAudioEnd() const {
	// 再生していなければ終了
	if (playHandle == -1) return true;
	return CheckSoundMem(playHandle) == 0;
}

void AudioInstance::SetClassID() {
	classID = typeid(this);
}
