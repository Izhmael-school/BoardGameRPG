#pragma once
#include "Vector2.h"

class MySprite {
public:

    /// <summary>
    /// 画像を座標の中心に描画するための計算
    /// </summary>
    /// <param name="gHandle"></param>
    /// <param name="pos"></param>
    /// <param name="exRateX"></param>
    /// <param name="exRateY"></param>
    /// <returns></returns>
    static Vector2 ImageCenterPos(int gHandle, Vector2 pos, float exRateX = 1, float exRateY = 1);

    /// <summary>
    /// 画像を座標の中心に描画するための計算
    /// </summary>
    /// <param name="gHandle"></param>
    /// <param name="pos"></param>
    /// <param name="exRateX"></param>
    /// <param name="exRateY"></param>
    /// <returns></returns>
    static Vector2 ExpendImageCenterPos(Vector2 pos, int exRateX, int exRateY);


    /// <summary>
    /// 画像の頂点の座標を取得
    /// </summary>
    static Vector2 ImageTopPos(int gHandle, int exRateX = 1, int exRateY = 1);

    static Vector2 BoxCenterPos(float _x, float _y, float _w, float _h);

    /*
     * @brief 画像のサイズを取得
     */
    static Vector2 GetSpriteSize(int _graphHandle);
};

