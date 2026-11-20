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
	std::pair<bool, bool> press = { false,false };
};

struct UIInput {

	std::vector<std::pair<int, UIInputPress>> key;

	std::vector<std::pair<int, UIInputPress>> mouse;

	std::vector<std::vector<std::pair<int, UIInputPress>>> pad;

	Vector2 mousePos = VZero_2;

	UIInput() {
		key.emplace_back(std::pair(KEY_RETURN, UIInputPress()));
		key.emplace_back(std::pair(KEY_UP, UIInputPress()));
		key.emplace_back(std::pair(KEY_DOWN, UIInputPress()));
		key.emplace_back(std::pair(KEY_RIGHT, UIInputPress()));
		key.emplace_back(std::pair(KEY_LEFT, UIInputPress()));
		key.emplace_back(std::pair(KEY_ESCAPE, UIInputPress()));
		key.emplace_back(std::pair(KEY_SPACE, UIInputPress()));
		key.emplace_back(std::pair(KEY_B, UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_LEFT, UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_RIGHT, UIInputPress()));
		mouse.emplace_back(std::pair(MOUSE_MIDDLE, UIInputPress()));
		const int PLAYER_NUM = 4;
		pad.resize(PLAYER_NUM);
		for (int j = 0; j < PLAYER_NUM; j++) {
			for (int i = 0, max = 16; i < max; i++) {
				pad[j].emplace_back(std::pair(i, UIInputPress()));
			}
		}
	}

	bool IsKeyDown(int _key);

	bool IsMouseDown(int _mouse);

	bool IsPadDown(int _pad);

	bool IsPadDown(int _padNum,int _pad);
};

#endif