/*
 * @brief エフェクトを管理するクラス
 * @author Sekino
 */
#pragma once

#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "../../ManagerBase.h"
#include <memory>
#include <string>
#include <vector>
#include "Library/Vector/Vector3.h"
#include "Instance/EffectInstance.h"

class EffectInstance;
class EffectResourceManager;

class EffectManager : public ManagerBase {
private:
	std::vector<EffectPtr> instances;	// 管理下にあるインスタンス
public:
	EffectResourceManager& pEffectResourceManager;// 読み込み用

public:
	EffectManager(EffectResourceManager& _resourceManager);

	/*
	 * @brief 生成
	 */
	EffectPtr Play(const std::string& _effectName, const Vector3& _pos, float _scale = 1.0f, const Vector3& _rot = VZero);

	/*
	 * @brief 更新
	 */
	void Update(float _t) override;

	/*
	 * @brief 描画
	 */
	void Render() override;

	/*
	 * @brief インスタンスの全削除
	 */
	void Clean();

	/*
	 * @brief 管理してるインスタンスの数
	 */
	int GetInstanceCount() const { return instances.size(); }
};
#endif