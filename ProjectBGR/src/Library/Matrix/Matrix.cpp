#include "Matrix.h"

Matrix::Matrix(float _0x0, float _0x1, float _0x2, float _0x3, float _1x0, float _1x1, float _1x2, float _1x3, float _2x0, float _2x1, float _2x2, float _2x3, float _3x0, float _3x1, float _3x2, float _3x3)
	:m() {
	m[0][0] = _0x0; m[0][1] = _0x1; m[0][2] = _0x2; m[0][3] = _0x3;
	m[1][0] = _1x0; m[1][1] = _1x1; m[1][2] = _1x2; m[1][3] = _1x3;
	m[2][0] = _2x0; m[2][1] = _2x1; m[2][2] = _2x2; m[2][3] = _2x3;
	m[3][0] = _3x0; m[3][1] = _3x1; m[3][2] = _3x2; m[3][3] = _3x3;
}

Matrix Matrix::GetTranslation(Vector3 _pos) {
	Matrix mat = MIdentity;

	mat.m[3][0] = _pos.x;
	mat.m[3][1] = _pos.y;
	mat.m[3][2] = _pos.z;

	return mat;
}

Matrix Matrix::GetRotationX(float xRadian) {
	Matrix mat = MIdentity;

	float c = cosf(xRadian);
	float s = sinf(xRadian);

	mat.m[1][1] = c;
	mat.m[1][2] = s;
	mat.m[2][1] = -s;
	mat.m[2][2] = c;

	//| 1     0     0     0 |
	//| 0   cosﾎｸ  sinﾎｸ  0 |
	//| 0  -sinﾎｸ  cosﾎｸ  0 |
	//| 0     0     0     1 |

	return mat;
}

Matrix Matrix::GetRotationY(float yRadian) {
	Matrix mat = MIdentity;

	float c = cosf(yRadian);
	float s = sinf(yRadian);

	mat.m[0][0] = c;
	mat.m[0][2] = -s;
	mat.m[2][0] = s;
	mat.m[2][2] = c;

	//| cosﾎｸ  0  -sinﾎｸ  0 |
	//|   0    1    0     0 |
	//| sinﾎｸ  0   cosﾎｸ  0 |
	//|   0    0    0     1 |

	return mat;
}

Matrix Matrix::GetRotationZ(float zRadian) {
	Matrix mat = MIdentity;

	float c = cosf(zRadian);
	float s = sinf(zRadian);

	mat.m[0][0] = c;
	mat.m[0][1] = s;
	mat.m[1][0] = -s;
	mat.m[1][1] = c;

	//| cosﾎｸ  sinﾎｸ  0   0 |
	//|-sinﾎｸ  cosﾎｸ  0   0 |
	//|   0     0     1   0 |
	//|   0     0     0   1 |

	return mat;
}

Matrix Matrix::GetRotationXYZ(Matrix _x, Matrix _y, Matrix _z) {
	return MMult(MMult(_z, _x), _y);
}

Matrix Matrix::GetScale(Vector3 _scale) {
	Matrix mat = MIdentity;

	mat.m[0][0] = _scale.x;
	mat.m[1][1] = _scale.y;
	mat.m[2][2] = _scale.z;

	return mat;
}

Matrix Matrix::MMult(Matrix _mat1, Matrix _mat2) {
	Matrix mat = MZero;

	for (int i = 0; i < FOUR; i++) {
		for (int j = 0; j < FOUR; j++) {
			for (int k = 0; k < FOUR; k++) {
				mat.m[i][j] += _mat1.m[i][k] * _mat2.m[k][j];
			}
		}
	}

	return mat;
}
