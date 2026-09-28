#include "Vector3.h"
#include "Vector2.h"
#include <math.h>
#include <algorithm>

Vector3::Vector3() 
	:x(0.0f)
	,y(0.0f)
	,z(0.0f)
{
}

Vector3::Vector3(float _x, float _y, float _z)
	:x(_x)
	, y(_y)
	, z(_z) {
}

float Vector3::Magnitude() const {
	return sqrtf(SqrMagnitude());
}

float Vector3::SqrMagnitude() const {
	return (x * x) + (y * y) + (z * z);
}

Vector3 Vector3::Normalized() const {
	float mag = Magnitude();
	if (mag == 0.0f) return VZero;

	// 0髯､邂怜ｯｾ遲・
	float nx = x != 0.0f ? x / mag : 0.0f;
	float ny = y != 0.0f ? y / mag : 0.0f;
	float nz = z != 0.0f ? z / mag : 0.0f;

	return Vector3(nx, ny, nz);
}

float Vector3::SqrMagnitude(Vector3 _vec) {
	return (_vec.x * _vec.x) + (_vec.y * _vec.y) + (_vec.z * _vec.z);
}

float Vector3::SqrMagnitude(float _x, float _y, float _z) {
	return (_x * _x) + (_y * _y) + (_z * _z);
}

std::array<float, 3> Vector3::GetArray() const {
	return std::array<float, 3>({ x,y,z });
}

Vector3 Vector3::Angle(Vector3 _from, Vector3 _to) {
	return Vector3(_from.x - _to.x, _from.y - _to.y, _from.z - _to.z);
}

float Vector3::Dot(Vector3 _vec1, Vector3 _vec2) {
	float x = _vec1.x * _vec2.x;
	float y = _vec1.y * _vec2.y;
	float z = _vec1.z * _vec2.z;

	return x + y + z;
}

Vector3 Vector3::Cross(Vector3 _vec1, Vector3 _vec2) {
	float x = _vec1.y * _vec2.z - _vec1.z * _vec2.y;
	float y = _vec1.x * _vec2.z - _vec1.z * _vec2.x;
	float z = _vec1.y * _vec2.x - _vec1.x * _vec2.y;

	return Vector3(x,y,z);
}

float Vector3::Distance(Vector3 _vec1, Vector3 _vec2) {
	return _vec1.Magnitude() - _vec2.Magnitude();
}

Vector3 Vector3::Lerp(Vector3 _vec1, Vector3 _vec2, float _t) {
	_t = std::clamp(_t, 0.0f, 1.0f);
	if (_t == 0.0f) return _vec1;
	if (_t == 1.0f) return _vec2;

	float lx = _vec1.x + (_vec2.x - _vec1.x) * _t;
	float ly = _vec1.y + (_vec2.y - _vec1.y) * _t;
	float lz = _vec1.z + (_vec2.z - _vec1.z) * _t;

	return Vector3(lx,ly,lz);
}

Vector3 Vector3::Max(Vector3 _vec1, Vector3 _vec2) {
	float mx = std::max(_vec1.x, _vec2.x);
	float my = std::max(_vec1.y, _vec2.y);
	float mz = std::max(_vec1.z, _vec2.z);
	return Vector3(mx,my,mz);
}

Vector3 Vector3::Min(Vector3 _vec1, Vector3 _vec2) {
	float mx = std::min(_vec1.x, _vec2.x);
	float my = std::min(_vec1.y, _vec2.y);
	float mz = std::min(_vec1.z, _vec2.z);
	return Vector3(mx, my, mz);
}

Vector3 Vector3::VAdd(Vector3 _vec1, Vector3 _vec2) {
	Vector3 vec = VZero;
	vec.x = _vec1.x + _vec2.x;
	vec.y = _vec1.y + _vec2.y;
	vec.z = _vec1.z + _vec2.z;

	return vec;
}

Vector3 Vector3::VAdd(Vector3 _vec, float _value) {
	Vector3 vec = VZero;
	vec.x = _vec.x + _value;
	vec.y = _vec.y + _value;
	vec.z = _vec.z + _value;

	return vec;
}

Vector3 Vector3::VSub(Vector3 _vec1, Vector3 _vec2) {
	Vector3 vec = VZero;
	vec.x = _vec1.x - _vec2.x;
	vec.y = _vec1.y - _vec2.y;
	vec.z = _vec1.z - _vec2.z;

	return vec;
}

Vector3 Vector3::VSub(Vector3 _vec, float _value) {
	Vector3 vec = VZero;
	vec.x = _vec.x - _value;
	vec.y = _vec.y - _value;
	vec.z = _vec.z - _value;

	return vec;
}

Vector3 Vector3::VMult(Vector3 _vec1, Vector3 _vec2) {
	Vector3 vec = VZero;
	vec.x = _vec1.x * _vec2.x;
	vec.y = _vec1.y * _vec2.y;
	vec.z = _vec1.z * _vec2.z;

	return vec;
}

Vector3 Vector3::VScale(Vector3 _vec, float _scale) {
	Vector3 vec = VZero;
	vec.x = _vec.x * _scale;
	vec.y = _vec.y * _scale;
	vec.z = _vec.z * _scale;

	return vec;
}

Vector3 Vector3::operator+(const Vector2& _v) const {
	return { x + _v.x,y + _v.y,0 };
}

Vector3 Vector3::operator+=(const Vector2& _v) const {
	return { x + _v.x,y + _v.y,0 };
}

Vector3 Vector3::operator-(const Vector2& _v) const {
	return { x - _v.x,y - _v.y,0 };
}

Vector3 Vector3::operator-=(const Vector2& _v) const {
	return { x - _v.x,y - _v.y,0 };
}

Vector3 Vector3::operator*(const Vector2& _v) const {
	return { x * _v.x,y * _v.y,0 };
}

Vector3 Vector3::operator*=(const Vector2& _v) const {
	return { x * _v.x,y * _v.y,0 };
}

Vector3 Vector3::operator=(const Vector2& _v) const {
	return { _v.x, _v.y,0 };
}

bool Vector3::operator==(const Vector2& _v) const {
	return { x == _v.x && y == _v.y };
}
