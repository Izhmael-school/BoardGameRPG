#include "EffectInstance.h"
//#include "EffekseerForDXLib.h"
#include "Manager/Resource/ResourceBase.h"
#include "ConversionVECTOR.h"

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
	// 莠碁㍾蜀咲函遖∵ｭ｢
	if (!IsEffectEnd()) return false;
	// 繝ｪ繧ｽ繝ｼ繧ｹ縺檎┌縺代ｌ縺ｰ蟶ｰ繧・
	if (!resource) return false;
	// 繧ｨ繝輔ぉ繧ｯ繝亥・逕・
	//playHandle = PlayEffekseer3DEffect(resource->GetHandle());
	// 蜀咲函縺ｧ縺阪↑縺九▲縺溘ｉ蟶ｰ繧・
	if (playHandle == -1) return false;
	auto transform = GetTransform();
	// 蠎ｧ讓吶ｒ險ｭ螳・

	VECTOR posV = ConversionVECTOR::Vector3ToVECTOR(_pos);
	//SetPosPlayingEffekseer3DEffect(playHandle, posV.x, posV.y, posV.z);
	transform->SetPosition(_pos);
	// 蝗櫁ｻ｢繧定ｨｭ螳・
	VECTOR rotV = ConversionVECTOR::Vector3ToVECTOR(_rot);
	//SetRotationPlayingEffekseer3DEffect(playHandle, rotV.x, rotV.y, rotV.z);
	transform->SetRotation(_rot);
	// 諡｡邵ｮ繧定ｨｭ螳・
	//VSetScalePlayingEffekseer3DEffect(playHandle, _scale, _scale, _scale);
	transform->SetScale(_scale);	
	
	return true;
}

void EffectInstance::Stop() {
	// 蜀咲函縺励※辟｡縺代ｌ縺ｰ蟶ｰ繧・
	if (IsEffectEnd()) return;
	// 蛛懈ｭ｢
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
