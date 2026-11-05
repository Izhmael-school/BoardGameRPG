/*
 * @brief cassertを使いやすくしたもの
 * @author Sekino
 */
#pragma once

#ifndef _MYASSERT_H_
#define _MYASSERT_H_

#include <string>

class MyAssert {
public:
	/*
	 * @brief 環境関係なく警告を出す
	 */
	static void Assert(const std::string& _text = "");

	/*
	 * @brief デバッグ環境でのみ警告を出す
	 */
	static void DebugAssert( const std::string& _text = "");
};

#endif