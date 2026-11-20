/*
 * @brief UIが参照するものをここで管理する
 * `author Sekino
 */
#pragma once
#ifndef _UICONTEXT_H_
#define _UICONTEXT_H_

#include <unordered_map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include "Assert/MyAssert.h"

struct UIValue {
	void* ptr;
	std::type_index type;

	UIValue(void* _ptr,std::type_index _type)
		:ptr(_ptr)
		,type(_type)
	{}

	UIValue() 
		:ptr(nullptr)
		,type(typeid(nullptr))
	{};
};

class UIContext {
private:
	std::unordered_map<std::string, UIValue> values;

public:
	/*
	 * @brief UIが使う変数を保持
	 */
	template<typename T>
	void Register(const std::string& _name, T* _value);

	template<typename T>
	T* Get(const std::string& _name);
};


template<typename T>
inline void UIContext::Register(const std::string& _name, T* _value) {
#if _DEBUG
	if (values.contains(_name))
		MyAssert::DebugAssert("同名の登録");
#endif
	values[_name] = { _value ,typeid(T)};
}

template<typename T>
inline T* UIContext::Get(const std::string& _name) {

	auto itr = values.find(_name);
	// なければnull
	if (itr == values.end()) return nullptr;
	// 型が違ったらnull
	std::type_index value = itr->second.type;
	if (value != typeid(T)) return nullptr;

	return static_cast<T*>(itr->second.ptr);
}

#endif // !_UICONTEXT_H_