#include "EffectManager.h"
#include "Manager/Resource/Effect/EffectResourceManager.h"
//#include "EffekseerForDXLib.h"

EffectManager::EffectManager(EffectResourceManager& _resourceManager)
    :pEffectResourceManager(_resourceManager)
    ,instances()
{
}

EffectPtr EffectManager::Play(const std::string& _effectName, const Vector3& _pos, float _scale, const Vector3& _rot) {
    // 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜿門ｾ・
    auto resource = pEffectResourceManager.GetResource(_effectName);
    // 繝ｪ繧ｽ繝ｼ繧ｹ縺檎┌縺代ｌ縺ｰ蟶ｰ繧・
    if (!resource) return nullptr;
    // 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ逕滓・
    auto instance = std::make_shared<EffectInstance>(resource);
    // 蜀咲函螟ｱ謨励＠縺溘ｉ蟶ｰ繧・
    if (!instance->Play(_pos, _scale, _rot)) return nullptr;
    // 蜀咲函縺ｧ縺阪◆繧蛾・蛻励↓蜈･繧後ｋ
    instances.push_back(instance);

    return instance;
}

void EffectManager::Update(float _t) {
    // Effekseer縺ｮ譖ｴ譁ｰ
    //UpdateEffekseer3D();

    // 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ譖ｴ譁ｰ
    for (auto& instance : instances) {
        instance->Update(_t);
    }

    // 蜀咲函縺檎ｵゅｏ縺｣縺溘ｉ豸医☆
    std::erase_if(instances, [](EffectPtr _instance) {
        return _instance->IsEffectEnd();
    });
}

void EffectManager::Render() {
    // Effekseer縺ｮ謠冗判
    //DrawEffekseer3D();
}

void EffectManager::Clean() {
    for (auto& instance : instances) {
        instance->Stop();
    }
    instances.clear();
    instances.shrink_to_fit();
}
