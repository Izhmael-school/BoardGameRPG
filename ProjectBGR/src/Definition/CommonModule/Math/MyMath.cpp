#include "MyMath.h"
#include "DxLib.h"

constexpr float PI = 3.1415926535897932384626433832795028841971f;

float MyMath::Rad2Deg(float _degree) {
	return(_degree * (180.0f / PI));
}

float MyMath::Deg2Rad(float _radian) {
	return (_radian * (PI / 180.0f));
}

int MyMath::Random(int min, int max) {
	return (min)+GetRand(max - min); 
}