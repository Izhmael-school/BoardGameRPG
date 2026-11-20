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

bool UIInput::IsPadDown(int _pad) {
    for (auto& p : pad) {
        for (auto& input : p) {
            if (input.first != _pad) continue;

            UIInputPress p = input.second;

            return p.press.first && !p.press.second;
        }
    }
    return false;
}

bool UIInput::IsPadDown(int _padNum, int _pad) {
    
    if (_padNum < 0 || _padNum > pad.size()) return false;

    for (auto& input : pad[_padNum]) {
        if (input.first != _pad) continue;

        UIInputPress p = input.second;

        return p.press.first && !p.press.second;
    }
    return false;
}
