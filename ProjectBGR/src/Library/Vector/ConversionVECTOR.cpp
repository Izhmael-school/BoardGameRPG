#include "ConversionVECTOR.h"

VECTOR ConversionVECTOR::Vector3ToVECTOR(Vector3 _vec) {
    VECTOR vec = VGet(0,0,0);

    vec.x = _vec.x;
    vec.y = _vec.y;
    vec.z = _vec.z;

    return vec;
}

Vector3 ConversionVECTOR::VECTORToVector3(VECTOR _vec) {
    Vector3 vec = VZero;

    vec.x = _vec.x;
    vec.y = _vec.y;
    vec.z = _vec.z;

    return vec;
}