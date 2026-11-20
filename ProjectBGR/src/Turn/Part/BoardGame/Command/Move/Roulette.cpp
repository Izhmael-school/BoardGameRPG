#include "Roulette.h"
#include "Manager/Input/InputManager.h"
#include "Math/MyMath.h"
#include <cmath>

Roulette::Roulette()
	:angle(0)
	, controllerPort(-1)
	, isRouletteStop(false)
	, isStop(false)
	, rotateValue(5000)
	, decelecation(0.0f) {
}

void Roulette::RouletteStart(int _controllerPort) {
	controllerPort = _controllerPort;
	isStop = false;
	rotateValue = 5000;
	angle = 0;
	isRouletteStop = false;
	decelecation = 0.0f;
}

void Roulette::Update(float _t) {

	// Aボタンを押したら徐々に止まるようにする
	if (InputManager::GetInstance().IsButtonDown(controllerPort, XINPUT_A) && !isStop) {
		isStop = true;
		decelecation = MyMath::Random(1000, 4500);
	}

	if (isStop) {
		rotateValue -= decelecation * _t;
		rotateValue = std::max(rotateValue, 0.0f);
	}

	// 回転を加算
	angle += rotateValue * _t;

	// １回転したらangleを0にする
	angle = std::fmod(angle, 360.0f);

	// 一定数値以下なら止まったことにする
	if (rotateValue <= 0.001 && isStop) {
		isRouletteStop = true;
	}
	return;
}

int Roulette::GetRouletteNum(int _cellCount) {
	float cellAngle = 360.0f / _cellCount;

	angle = std::fmod(angle, 360.0f);
	if (angle < 0.0f)
		angle += 360.0f;

	int index = static_cast<int>(std::floor(angle / cellAngle));

	return index % _cellCount;
}
