#pragma once

#include <string>
#include "Vector2.h"

struct UICreateMinimumNeedInfo {
	Vector2 pos = VZero_2;
	std::string name = "";
	std::string parentName = "";
	bool isActive = true;

	UICreateMinimumNeedInfo(Vector2 _pos, std::string _name, std::string _parentName, bool _isActive)
		:pos(_pos)
		,name(_name)
		,parentName(_parentName)
		,isActive(_isActive)
	{}
};