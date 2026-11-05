#include "MySprite.h"
#include "DxLib.h"

Vector2 MySprite::ImageCenterPos(int gHandle, Vector2 pos, float exRateX, float exRateY) {
    int x, y;
    GetGraphSize(gHandle, &x, &y);

    pos.x -= (float)(x * exRateX) / 2;
    pos.y -= (float)(y * exRateY) / 2;

    return pos;
}

Vector2 MySprite::ExpendImageCenterPos(Vector2 pos, int exRateX, int exRateY) {

    pos.x -= (float)(exRateX) / 2;
    pos.y -= (float)(exRateY) / 2;

    return pos;
}

Vector2 MySprite::ImageTopPos(int gHandle, int exRateX, int exRateY) {
    int x, y;
    GetGraphSize(gHandle, &x, &y);

    x = (int)(x * exRateX);
    y = (int)(y * exRateY);

    return Vector2(x, y);
}

Vector2 MySprite::BoxCenterPos(float _x, float _y, float _w, float _h) {
    float x, y;

    x = _x - (_w / 2);
    y = _y - (_h / 2);

    return  Vector2(x,y);
}

Vector2 MySprite::GetSpriteSize(int _graphHandle) {
    int x, y;
    GetGraphSize(_graphHandle, &x, &y);
    return Vector2(x,y);
}
