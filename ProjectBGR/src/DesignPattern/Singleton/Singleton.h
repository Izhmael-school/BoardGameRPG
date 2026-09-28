/*
 * @brief シングルトンクラス
 * @author Sekino
 */
#pragma once
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T>
class Singleton {
public:

	/// <summary>
	/// 参照
	/// </summary>
	/// <returns></returns>
	inline static T& GetInstance() {
		static T instance;
		return instance;
	}

	// コピーの制限
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	Singleton() = default;
	~Singleton() = default;

};
#endif