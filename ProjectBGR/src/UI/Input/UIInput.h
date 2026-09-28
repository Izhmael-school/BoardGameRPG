/*
 * @brief UIに入力処理を持たせないための構造体
 * @author Sekino
 */
#pragma once
#ifndef _UIINPUT_H_
#define _UIINPUT_H_

#include <iostream>
#include <vector>
#include "Vector2.h"
#include "Definition/Const/KeyInputConst.h"

struct UIInputPress {
	std::pair<bool, bool> press = {false,false};
};

struct UIInput {

	std::vector<std::pair<int, UIInputPress>> key;

	UIInput() {
		key.emplace_back(std::pair( KEY_RETURN,UIInputPress()));
		key.emplace_back(std::pair( KEY_UP,UIInputPress()));
		key.emplace_back(std::pair( KEY_DOWN,UIInputPress()));
		key.emplace_back(std::pair( KEY_RIGHT,UIInputPress()));
		key.emplace_back(std::pair( KEY_LEFT,UIInputPress()));
		key.emplace_back(std::pair( KEY_ESCAPE,UIInputPress()));
		key.emplace_back(std::pair( KEY_SPACE,UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_LEFT, UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_RIGHT, UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_MIDDLE, UIInputPress()));
	}

	bool IsKeyDown(int _key);

	bool IsMouseDown(int _mouse);
	
	std::vector<std::pair<int, UIInputPress>> mouse;

	Vector2 mousePos = VZero_2;
};

#endif