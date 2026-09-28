/*
 * @brief キャラクターオブジェクトクラス
 */
#pragma once
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../GameObject.h"

class CharacterData;

class Character : public GameObject {
private:
	CharacterData* myData;
	bool isBattling;

public:
	Character(int _modelHandel, std::string _name,CharacterData* _myData);

	~Character() = default;

	void Update(float _t) override;

	void Render() override;

	void SetClassID() override;
};

#endif // !_CHARACTER_H_