#include "Collider.h"
#include "Manager/Collision/CollisionManager.h"
#include "GameObject/GameObject.h"
#include "Component/Transform/Transform.h"

Collider::Collider(GameObject* _attachObject, CollisionManager* _colliderManager, const ColliderShape& _shape, CollisionLayer _layer)
	:ComponentBase(_attachObject),
	pManager(_colliderManager) {
	handle = pManager->Create(_shape, _layer, this);
	// 蠎ｧ讓吶・縺吶ｊ蜷医ｏ縺・
	lastPos = _attachObject->GetTransform()->GetPosition();
	pManager->SetWorldPos(handle, lastPos);
}

Collider::~Collider() {
	if (pManager)
		pManager->Destroy(handle);
}

void Collider::Update(float _t) {
	if (!IsActive()) return;

	Vector3 currentPos = attachObject->GetTransform()->GetPosition();

	// 蠎ｧ讓吶′螟牙喧縺励※縺ｪ縺代ｌ縺ｰ蟶ｰ繧・
	if (currentPos.x == lastPos.x && currentPos.y == lastPos.y && currentPos.z == lastPos.z)
		return;

	// 蠎ｧ讓吶・縺吶ｊ蜷医ｏ縺・
	pManager->SetWorldPos(handle, currentPos);
	lastPos = currentPos;
}

void Collider::Render() {
}

bool Collider::IsHit() const {
	return GetColliderData().currentHit;
}

bool Collider::IsPrevHit() const {
	return GetColliderData().prevHit;
}

void Collider::Enter(ColliderData _pSelf, ColliderData _pOther) {
	int triggerCount = _pSelf.isTrigger + _pOther.isTrigger;

	// 縺ｩ縺｣縺｡繧ゅヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 2) return;

	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺後ヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 1 && _pSelf.isTrigger) {
		_pSelf.owner->OnTriggerEnter(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺ｯ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 1 && !_pSelf.isTrigger) {
		_pSelf.owner->OnCollisionEnter(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨ｂ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 0) {
		_pSelf.owner->OnCollisionEnter(_pSelf,_pOther);
		return;
	}
}

void Collider::Stay(ColliderData _pSelf, ColliderData _pOther) {
	int triggerCount = _pSelf.isTrigger + _pOther.isTrigger;

	// 縺ｩ縺｣縺｡繧ゅヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 2) return;

	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺後ヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 1 && _pSelf.isTrigger) {
		_pSelf.owner->OnTriggerStay(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺ｯ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 1 && !_pSelf.isTrigger) {
		_pSelf.owner->OnCollisionStay(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨ｂ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 0) {
		_pSelf.owner->OnCollisionStay(_pSelf, _pOther);
		return;
	}
}

void Collider::Exit(ColliderData _pSelf, ColliderData _pOther) {
	int triggerCount = _pSelf.isTrigger + _pOther.isTrigger;

	// 縺ｩ縺｣縺｡繧ゅヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 2) return;

	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺後ヨ繝ｪ繧ｬ繝ｼ
	if (triggerCount == 1 && _pSelf.isTrigger) {
		_pSelf.owner->OnTriggerExit(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨°縺後ヨ繝ｪ繧ｬ繝ｼ縺ｧ縺九▽閾ｪ霄ｫ縺ｯ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 1 && !_pSelf.isTrigger) {
		_pSelf.owner->OnCollisionExit(_pSelf, _pOther);
		return;
	}
	// 縺ｩ縺｡繧峨ｂ繝医Μ繧ｬ繝ｼ縺ｧ縺ｯ縺ｪ縺・
	else if (triggerCount == 0) {
		_pSelf.owner->OnCollisionExit(_pSelf, _pOther);
		return;
	}
}

void Collider::SetActive(bool _isActive) {
	isActive = _isActive;
	GetColliderData().isEnable = _isActive;
}

CollisionLayer Collider::GetLayer() const {
	return GetColliderData().layer;
}

void Collider::SetLayer(CollisionLayer _layer) {
	GetColliderData().layer = _layer;
}

ColliderData& Collider::GetColliderData() const {
	return pManager->GetCollider(handle);
}
