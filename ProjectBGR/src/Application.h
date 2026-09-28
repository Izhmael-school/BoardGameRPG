/*
 * @brief 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ繧ｯ繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "DesignPattern/Singleton/Singleton.h"
#include "ImGuiManager.h"
#include <memory>

constexpr int FPS = 60;
constexpr double FRAME_TIME = 1.0 / FPS;

class ModelResourceManager;
class EffectResourceManager;
class AudioResourceManager;

class Application : public Singleton<Application>  {
private:
	ImGuiManager imgui;

	bool isGameEnd; // 繧ｲ繝ｼ繝邨ゆｺ・ヵ繝ｩ繧ｰ

	// リソース系はここで全管理
	std::unique_ptr<ModelResourceManager> modelResourceManager;
	std::unique_ptr<EffectResourceManager> effectResourceManager;
	std::unique_ptr<AudioResourceManager> audioResourceManager;

public:
	Application();
	~Application();

private:
	/*
	 * @brief 蛻晄悄蛹・
	 */
	int Init();

	/*
	 * @brief DxLib縺ｮ蛻晄悄蛹・
	 */
	int DxLibInit();

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	bool Update();

	/*
	 * @brief 謠冗判
	 */
	void Render();

	/*
	 * @brief リソース管理インスタンスの生成とリソースのロード
	 */
	void ResourceLoad();

	/*
	 * @brief 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜑企勁
	 */
	void ResourceDelete();

	/*
	 * @brief 邨ゆｺ・燕蜃ｦ逅・
	 */
	void End();

	/*
	 * @brief DxLib縺ｮ邨ゆｺ・燕蜃ｦ逅・
	 */
	void DxLibEnd();

public:
	/*
	 * @brief 繧ｲ繝ｼ繝縺ｮ繝｡繧､繝ｳ繝ｫ繝ｼ繝・
	 */
	void Run();

	/*
	 * @brief 繧ｲ繝ｼ繝邨ゆｺ・
	 */
	void GameEnd() { isGameEnd = true; }

	// ---各種リソースのゲッター---
	ModelResourceManager* GetModelResourceManager() const { return modelResourceManager.get(); }
	EffectResourceManager* GetEffectResourceManager() const { return effectResourceManager.get(); }
	AudioResourceManager* GetAudioResourceManager() const { return audioResourceManager.get(); }
};

#endif // !_APPLICATION_H_
