#include "UIInput.h"

bool UIInput::IsKeyDown(int _key) {
    for (auto& k : key) {
        if (k.first != _key) continue;

        UIInputPress p = k.second;

        return p.press.first && !p.press.second;
    }
    return false;
}

bool UIInput::IsMouseDown(int _mouse) {
    for (auto& m : mouse) {
        if (m.first != _mouse) continue;

        UIInputPress p = m.second;

        return p.press.first && !p.press.second;
    }
    return false;
}
