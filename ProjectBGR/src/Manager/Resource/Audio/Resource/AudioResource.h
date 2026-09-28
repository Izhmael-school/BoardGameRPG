/*
 * @brief 繧ｪ繝ｼ繝・ぅ繧ｪ繧定ｪｭ縺ｿ霎ｼ繧繝ｪ繧ｽ繝ｼ繧ｹ繧ｯ繝ｩ繧ｹ
 * @author Sekino
 */

#pragma once
#ifndef _AUDIORESOURCE_H_
#define _AUDIORESOURCE_H_

#include "../../ResourceBase.h"

class AudioResource : public ResourceBase {
	bool is3D;

public:
	AudioResource(const std::string& _name, const std::string& _path, bool _is3D = false);
	~AudioResource() override;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	bool Load() override;

	/*
	 * @brief 3D髻ｳ貅舌°縺ｩ縺・°
	 */
	const bool Is3D() const { return is3D; }

};
#endif

#include <memory>
using AudioResourcePtr = std::shared_ptr<AudioResource>;