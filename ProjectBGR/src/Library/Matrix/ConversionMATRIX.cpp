#include "ConversionMATRIX.h"

MATRIX ConversionMATRIX::MatrixToMATRIX(Matrix _mat) {
    MATRIX mat = MGetIdent();

    for (int i = 0;i < FOUR;i++) {
        for (int j = 0;j < FOUR;j++) {
            mat.m[i][j] = _mat.m[i][j];
        }
    }

    return mat;
}

Matrix ConversionMATRIX::MATRIXToMatrix(MATRIX _mat) {
    Matrix mat = MIdentity;

    for (int i = 0;i < FOUR;i++) {
        for (int j = 0;j < FOUR;j++) {
            mat.m[i][j] = _mat.m[i][j];
        }
    }

    return mat;
}
