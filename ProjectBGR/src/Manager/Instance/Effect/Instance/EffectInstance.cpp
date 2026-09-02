#include "EffectInstance.h"
//#include "EffekseerForDXLib.h"
#include "Manager/Resource/ResourceBase.h"
#include "Vector/ConversionVECTOR.h"

EffectInstance::EffectInstance(std::shared_ptr<EffectResource> _effectResource)
	:InstanceBase(_effectResource)
	, playHandle(-1) {
}

void EffectInstance::Update(float _t) {
	GameObject::Update(_t);

	Vector3 pos = GetTransform()->GetPosition();
	VECTOR posV = ConversionVECTOR::Vector3ToVECTOR(pos);
	//SetPosPlayingEffekseer3DEffect(playHandle, posV.x, posV.y, posV.z);
	Vector3 rot = GetTransform()->GetRotation();
	VECTOR rotV = ConversionVECTOR::Vector3ToVECTOR(rot);
	//SetRotationPlayingEffekseer3DEffect(playHandle, rotV.x, rotV.y, rotV.z);
	Vector3 scale = GetTransform()->GetScale();
	VECTOR scaleV = ConversionVECTOR::Vector3ToVECTOR(scale);
	//SetScalePlayingEffekseer3DEffect(playHandle, scaleV.x, scaleV.y, scaleV.z);
}

void EffectInstance::Render() {
	GameObject::Render();
}

bool EffectInstance::Play(Vector3 _pos, float _scale, Vector3 _rot) {
	// 二重再生禁止
	if (!IsEffectEnd()) return false;
	// リソースが無ければ帰る
	if (!resource) return false;
	// エフェクト再生
	//playHandle = PlayEffekseer3DEffect(resource->GetHandle());
	// 再生できなかったら帰る
	if (playHandle == -1) return false;
	auto transform = GetTransform();
	// 座標を設定

	VECTOR posV = ConversionVECTOR::Vector3ToVECTOR(_pos);
	//SetPosPlayingEffekseer3DEffect(playHandle, posV.x, posV.y, posV.z);
	transform->SetPosition(_pos);
	// 回転を設定
	VECTOR rotV = ConversionVECTOR::Vector3ToVECTOR(_rot);
	//SetRotationPlayingEffekseer3DEffect(playHandle, rotV.x, rotV.y, rotV.z);
	transform->SetRotation(_rot);
	// 拡縮を設定
	//VSetScalePlayingEffekseer3DEffect(playHandle, _scale, _scale, _scale);
	transform->SetScale(_scale);	
	
	return true;
}

void EffectInstance::Stop() {
	// 再生して無ければ帰る
	if (IsEffectEnd()) return;
	// 停止
	//StopEffekseer3DEffect(playHandle);
	playHandle = -1;
}

const bool EffectInstance::IsEffectEnd() const {
	//return IsEffekseer3DEffectPlaying(playHandle) == -1;
	return true;
}

void EffectInstance::SetClassID() {
	classID = typeid(this);
}
