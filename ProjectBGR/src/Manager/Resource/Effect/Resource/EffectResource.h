/*
 * @brief 繧ｨ繝輔ぉ繧ｯ繝医ｒ隱ｭ縺ｿ霎ｼ繧繝ｪ繧ｽ繝ｼ繧ｹ繧ｯ繝ｩ繧ｹ
 * @author Sekino
 */

#pragma once
#ifndef _EFFECTRESOURCE_H_
#define _EFFECTRESOURCE_H_

#include "../../ResourceBase.h"
class EffectResource : public ResourceBase {
	float magnification;

public:
	EffectResource(const std::string& _name, const std::string& _path,float _magnification = 1.0f);
	~EffectResource() override;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	bool Load() override;

};
#endif