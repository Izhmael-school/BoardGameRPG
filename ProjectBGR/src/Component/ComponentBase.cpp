#include "ComponentBase.h"

ComponentBase::ComponentBase(GameObject* _attachObject) 
	:attachObject(_attachObject)
	,isActive(true)
{}

void ComponentBase::Update(float _t) {}

void ComponentBase::Render() {}
