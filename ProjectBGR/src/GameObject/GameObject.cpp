#include "GameObject.h"
#include "DxLib.h"
#include "Library/Matrix/ConversionMatrix.h"
#include "Library/Vector/ConversionVECTOR.h"

GameObject::GameObject(int _modelHandle, const std::string& _name, Vector3 _pos, Vector3 _rot, Vector3 _scale, Tag _tag) 
	:modelHandle(_modelHandle)
, tag(_tag)
, isActive(true)
, wantDelete(false)
, pTransform(nullptr)
, components()
, classID(typeid(this)) {
	// トランスフォームだけは絶対につける
	pTransform = AddComponent<Transform>();
	Start();
	size_t id = classID.hash_code();
}

GameObject::~GameObject() {
	// コンポーネントの削除
	std::erase_if(components, [this](std::unique_ptr<ComponentBase>& _component) {return true; });
}

void GameObject::Start() {
}

void GameObject::Update(float _t) {
	// コンポーネントの更新
	for (auto& c : components) {
		c->Update(_t);
	}
}

void GameObject::Render() {
	// コンポーネントの更新
	for (auto& c : components) {
		c->Render();
	}

	// モデルがあれば描画
	if (modelHandle != -1) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		MATRIX matrix = ConversionMATRIX::MatrixToMATRIX(pTransform->GetMatrix());
		MV1SetMatrix(modelHandle, matrix);
		MV1DrawModel(modelHandle);
	}
	// モデルが無ければデバッグでのみ赤点を描画
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
	// 同じなら帰る
	if (isActive == _isActive) return;
	isActive = _isActive;
	// 状態に応じて関数に入る
	if (isActive)
		Enable();
	else
		Disable();
}

bool GameObject::CompareTag(Tag _tag) const {
	return tag == _tag;
}
