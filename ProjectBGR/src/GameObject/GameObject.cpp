#include "GameObject.h"
#include "DxLib.h"
#include "Library/Matrix/ConversionMatrix.h"
#include "Library/Vector/ConversionVECTOR.h"

GameObject::GameObject(int _modelHandle, const std::string& _name, Vector3 _pos, Vector3 _rot, Vector3 _scale, Tag _tag)
	:modelHandle(_modelHandle)
	, tag(_tag)
	, name(_name)
	, isActive(true)
	, wantDelete(false)
	, pTransform(nullptr)
	, components()
	, classID(typeid(this)) {
	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝縺縺代・邨ｶ蟇ｾ縺ｫ縺､縺代ｋ
	pTransform = AddComponent<Transform>();
	Start();
	size_t id = classID.hash_code();
}

GameObject::~GameObject() {
	// 繧ｳ繝ｳ繝昴・繝阪Φ繝医・蜑企勁
	std::erase_if(components, [this](std::unique_ptr<ComponentBase>& _component) {return true; });
}

void GameObject::Start() {
}

void GameObject::Update(float _t) {
	// 繧ｳ繝ｳ繝昴・繝阪Φ繝医・譖ｴ譁ｰ
	for (auto& c : components) {
		c->Update(_t);
	}
}

void GameObject::Render() {
	// 繧ｳ繝ｳ繝昴・繝阪Φ繝医・譖ｴ譁ｰ
	for (auto& c : components) {
		c->Render();
	}

	// 繝｢繝・Ν縺後≠繧後・謠冗判
	if (modelHandle != -1) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		MATRIX matrix = ConversionMATRIX::MatrixToMATRIX(pTransform->GetMatrix());
		MV1SetMatrix(modelHandle, matrix);
		MV1DrawModel(modelHandle);
	}
	// 繝｢繝・Ν縺檎┌縺代ｌ縺ｰ繝・ヰ繝・げ縺ｧ縺ｮ縺ｿ襍､轤ｹ繧呈緒逕ｻ
	else {
#if _DEBUG
		VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(pTransform->GetPosition());
		DrawSphere3D(pos, 10, 16, 0xff0000, 0xff0000, TRUE);
#endif
	}
}

void GameObject::Setup() {
}

void GameObject::Cleanup() {
	MV1DeleteModel(modelHandle);
}

void GameObject::Enable() {
}

void GameObject::Disable() {
}

void GameObject::SetClassID() {
	classID = typeid(this);
}

void GameObject::OnTriggerEnter(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::OnTriggerStay(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::OnTriggerExit(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::OnCollisionEnter(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::OnCollisionStay(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::OnCollisionExit(ColliderData& _pSelf, ColliderData& _pOther) {
}

void GameObject::SetActive(bool _isActive) {
	// 蜷後§縺ｪ繧牙ｸｰ繧・
	if (isActive == _isActive) return;
	isActive = _isActive;
	// 迥ｶ諷九↓蠢懊§縺ｦ髢｢謨ｰ縺ｫ蜈･繧・
	if (isActive)
		Enable();
	else
		Disable();
}

bool GameObject::CompareTag(Tag _tag) const {
	return tag == _tag;
}

Vector3 GameObject::GetFramePos(std::string _frameName) {
	int frameIndex = MV1SearchFrame(modelHandle, _frameName.c_str());
	return ConversionVECTOR::VECTORToVector3(MV1GetFramePosition(modelHandle, frameIndex));
}

void GameObject::ChangeMaterialColor(std::string _frameName, float _r, float _g, float _b) {
	int frameIndex = MV1SearchFrame(modelHandle, _frameName.c_str());
	int meshIndex = MV1GetFrameMesh(modelHandle, frameIndex, 0);
	int matIndex = MV1GetMeshMaterial(modelHandle, meshIndex);
	MV1SetMaterialDifColor(modelHandle, matIndex, GetColorF(_r, _g, _b, 255));
}
