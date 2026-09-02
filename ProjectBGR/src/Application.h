/*
 * @brief アプリケーションクラス
 * @author Sekino
 */
#pragma once
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "DesignPattern/Singleton/Singleton.h"
#include "ImGuiManager.h"

constexpr int FPS = 60;
constexpr double FRAME_TIME = 1.0 / FPS;

class Application : public Singleton<Application>  {
private:
	ImGuiManager imgui;

	bool isGameEnd; // ゲーム終了フラグ
public:
	Application();
	~Application() = default;

private:
	/*
	 * @brief 初期化
	 */
	int Init();

	/*
	 * @brief DxLibの初期化
	 */
	int DxLibInit();

	/*
	 * @brief 更新
	 */
	bool Update();

	/*
	 * @brief 描画
	 */
	void Render();

	/*
	 * @brief リソースの削除
	 */
	void ResourceDelete();

	/*
	 * @brief 終了前処理
	 */
	void End();

	/*
	 * @brief DxLibの終了前処理
	 */
	void DxLibEnd();

public:
	/*
	 * @brief ゲームのメインループ
	 */
	void Run();

	/*
	 * @brief ゲーム終了
	 */
	void GameEnd() { isGameEnd = true; }

};

#endif // !_APPLICATION_H_
