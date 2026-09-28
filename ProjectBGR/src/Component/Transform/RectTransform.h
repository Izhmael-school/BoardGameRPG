/*
 * @brief UI用Transform
 * @author Sekino
 */
#pragma once
#ifndef _RECTTRANSFORM_H_
#define _RECTTRANSFORM_H_

#include "../ComponentBase.h"
#include "Vector2.h"

class RectTransform : public ComponentBase {
	Vector2 min,max;
	Vector2 anchoredPosition;
	Vector2 sizeDelta;
	Vector2 pivot;
	Vector2 offsetMin, offsetMax;
};

#endif