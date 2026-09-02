/*
 * @brief エフェクトのインスタンスを管理するクラス
 * @author Sekino
 */
#pragma once
#ifndef _EFFECTINSTANCE_H_
#define _EFFECTINSTANCE_H_

#include "../../InstanceBase.h"
#include "Manager/Resource/Effect/Resource/EffectResource.h"
#include "Vector/Vector3.h"

class EffectResource;

class EffectInstance : public InstanceBase {
private:
	int playHandle;	// リソースが持っているハンドル
	
public:
	EffectInstance(std::shared_ptr<EffectResource> _effectResource);
	~EffectInstance() = default;

	/*
	 * @brief 更新
	 */
	void Update(float _t) override;

	/*
	 * @brief 描画
	 */
	void Render() override;

	/*
	 * @brief 再生
	 */
	bool Play(Vector3 _pos,float _scale,Vector3 _rot = VZero);

	/*
	 * @brief 停止
	 */
	void Stop();

	/*
	 * @brief 再生が終わってるか
	 */
	const bool IsEffectEnd() const;

private:
	void SetClassID();
};
#endif // !_EFFECTINSTANCE_H_

// 別名
using EffectPtr = std::shared_ptr<EffectInstance>;