#include "InstanceBase.h"
#include <cassert>

InstanceBase::InstanceBase(ResourcePtr _resource)
	:resource(_resource)
	,wantDelete(false)
{
#if _DEBUG
	// 繝ｪ繧ｽ繝ｼ繧ｹ縺檎┌縺代ｌ縺ｰ隴ｦ蜻・
	assert(resource && "resouce is null");
#endif
}
