#include "MyMath.h"
#include "DxLib.h"
#include <cmath>

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

int MyMath::Max(int _v1, int _v2) {
	return _v1 > _v2 ? _v1 : _v2;
}

int MyMath::Min(int _v1, int _v2) {
	return _v1 < _v2 ? _v1 : _v2;
}

Vector3 MyMath::Lerp(Vector3 _before, Vector3 _after, float _t) {
	if (_t >= 1.0f) return _after;

	float lx = std::lerp(_before.x, _after.x, _t);
	float ly = std::lerp(_before.y, _after.y, _t);
	float lz = std::lerp(_before.z, _after.z, _t);

	return Vector3(lx, ly, lz);
}
