#include "Vector2.h"
#include "Vector3.h"
#include <algorithm>

Vector2 Vector2::operator+(const Vector3& _v) const {
	return Vector2(x + _v.x, y + _v.y);
}

Vector2 Vector2::operator+=(const Vector3& _v) const {
	return Vector2(x + _v.x, y + _v.y);
}

Vector2 Vector2::operator-(const Vector3& _v) const {
	return Vector2(x - _v.x, y - _v.y);
}

Vector2 Vector2::operator-=(const Vector3& _v) const {
	return Vector2(x - _v.x, y - _v.y);
}

Vector2 Vector2::operator*(const Vector3& _v) const {
	return Vector2(x * _v.x, y * _v.y);
}

Vector2 Vector2::operator*=(const Vector3& _v) const {
	return Vector2(x * _v.x, y * _v.y);
}

Vector2 Vector2::operator=(const Vector3& _v) const {
	return Vector2(_v.x,_v.y);
}

bool Vector2::operator==(const Vector3& _v) const {
	return  x == _v.x && y == _v.y;
}

Vector2::Vector2()
	:x(0.0f)
	,y(0.0f)
{}

Vector2::Vector2(float _x, float _y) 
	:x(_x)
	,y(_y)
{}

float Vector2::Magnitude() const {
	return sqrtf(SqrMagnitude());
}

float Vector2::SqrMagnitude() const {
	return (x * x) + (y * y);
}

Vector2 Vector2::Normalized() const {
	float mag = Magnitude();
	if (mag == 0.0f) return VZero_2;
	float nx = x != 0.0f ? x / mag : 0.0f;
	float ny = y != 0.0f ? y / mag : 0.0f;

	return Vector2(nx,ny);
}

float Vector2::SqrMagnitude(Vector2 _vec) {
	return (_vec.x * _vec.x) + (_vec.y * _vec.y);
}

float Vector2::SqrMagnitude(float _x, float _y, float _z) {
	return (_x * _x) + (_y * _y);
}

std::array<float, 2> Vector2::GetArray() const {
	return std::array<float, 2>({x,y});
}

Vector2 Vector2::Angle(Vector2 _from, Vector2 _to) {
	return Vector2(_from.x - _to.x, _from.y - _to.y);
}

float Vector2::Dot(Vector2 _vec1, Vector2 _vec2) {
	float x = _vec1.x * _vec2.x;
	float y = _vec1.y * _vec2.y;

	return x + y;
}

float Vector2::Distance(Vector2 _vec1, Vector2 _vec2) {
	return _vec1.Magnitude() - _vec2.Magnitude();;
}

Vector2 Vector2::Lerp(Vector2 _vec1, Vector2 _vec2, float _t) {
	_t = std::clamp(_t, 0.0f, 1.0f);
	if (_t == 0.0f) return _vec1;
	if (_t == 1.0f) return _vec2;

	float lx = _vec1.x + (_vec2.x - _vec1.x) * _t;
	float ly = _vec1.y + (_vec2.y - _vec1.y) * _t;

	return Vector2(lx, ly);
}

Vector2 Vector2::Max(Vector2 _vec1, Vector2 _vec2) {
	float mx = std::max(_vec1.x, _vec2.x);
	float my = std::max(_vec1.y, _vec2.y);
	return Vector2(mx, my);
}

Vector2 Vector2::Min(Vector2 _vec1, Vector2 _vec2) {
	float mx = std::min(_vec1.x, _vec2.x);
	float my = std::min(_vec1.y, _vec2.y);
	return Vector2(mx, my);
}

Vector2 Vector2::VAdd(Vector2 _vec1, Vector2 _vec2) {
	Vector2 vec = VZero_2;
	vec.x = _vec1.x + _vec2.x;
	vec.y = _vec1.y + _vec2.y;
	return vec;
}

Vector2 Vector2::VAdd(Vector2 _vec, float _value) {
	Vector2 vec = VZero_2;
	vec.x = _vec.x + _value;
	vec.y = _vec.y + _value;

	return vec;
}

Vector2 Vector2::VSub(Vector2 _vec1, Vector2 _vec2) {
	Vector2 vec = VZero_2;
	vec.x = _vec1.x - _vec2.x;
	vec.y = _vec1.y - _vec2.y;
	return vec;
}

Vector2 Vector2::VSub(Vector2 _vec, float _value) {
	Vector2 vec = VZero_2;
	vec.x = _vec.x - _value;
	vec.y = _vec.y - _value;

	return vec;
}

Vector2 Vector2::VMult(Vector2 _vec1, Vector2 _vec2) {
	Vector2 vec = VZero_2;
	vec.x = _vec1.x * _vec2.x;
	vec.y = _vec1.y * _vec2.y;
	return vec;
}

Vector2 Vector2::VScale(Vector2 _vec, float _scale) {
	Vector2 vec = VZero_2;
	vec.x = _vec.x - _scale;
	vec.y = _vec.y - _scale;

	return vec;
}
