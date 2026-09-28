#include "Character.h"
#include "Data/CharacterData.h"
#include "DxLib.h"
#include "ConversionVECTOR.h"
#include "Data/Player/PlayerData.h"

Character::Character(int _modelHandel,std::string _name, CharacterData* _myData)
	:GameObject(_modelHandel,_name)
	,myData(_myData) 
	,isBattling(false)
{
}

void Character::Update(float _t) {
	GameObject::Update(_t);
	
	if (myData->IsBattling()) return;
	// データ上の座標をもらう
	Vector3 mapPos = myData->GetMapPosition();
	// オブジェクト上の座標に変換
	GetTransform()->SetPosition(Vector3(mapPos.x * 200, 100, mapPos.y * -200));
}

void Character::Render() {
	GameObject::Render();
	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(GetTransform()->GetPosition());
}

void Character::SetClassID() {
	classID = typeid(this);
}
