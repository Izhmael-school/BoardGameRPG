#include "UIObject.h"

UIObject::UIObject()
    :position(VZero_2)
    ,isActive(true)
    ,parent(nullptr)
    ,children()
{
}

UIObject::~UIObject() {
    for (auto& child : children) {
        child.reset();
    }

    // remove null children
    std::erase_if(children, [](const std::unique_ptr<UIObject>& _child) { return !_child; });
}

void UIObject::Init() {
    // 固有の初期設定
    OnInit();

    // 子の初期設定
    if (children.empty()) return;

    for (auto& child : children) {
        child->Init();
    }
}

void UIObject::Update(float _t) {
    // 固有の更新処理
    OnUpdate(_t);

    // 子の更新処理
    if (children.empty()) return;

    for (auto& child : children) {
        child->Update(_t);
    }
}

void UIObject::Update(float _t, UIInput& _input) {
    // 固有の更新処理
    OnUpdate(_t,_input);

    // 子の更新処理
    if (children.empty()) return;

    for (auto& child : children) {
        child->Update(_t,_input);
    }
}

void UIObject::Render() {
    // 固有の描画処理
    OnRender();

    // 子の描画処理
    if (children.empty()) return;

    for (auto& child : children) {
        child->Render();
    }
}

void UIObject::End() {
    // 子の終了処理
    if (children.empty()) return;

    for (auto& child : children) {
        child->End();
    }

    // 固有の終了処理
    OnEnd();
}

Vector2 UIObject::GetWorldPosition() const {
    // 親がいなければ自身の座標を返す
    if (!parent)
        return position;

    return parent->GetWorldPosition() + position;
}

bool UIObject::IsActiveInParent() {

    if (!isActive) return false;
    if (parent) return parent->IsActiveInParent();

    return true;
}

void UIObject::AddChild(std::unique_ptr<UIObject> _child) {
    // 二重登録防止（ポインタ比較）
    auto itr = std::find_if(children.begin(), children.end(), [&](const std::unique_ptr<UIObject>& c) { return c.get() == _child.get(); });
    if (itr != children.end()) return;
    _child->parent = this;
    // 登録（ムーブ）
    children.emplace_back(std::move(_child));
}

std::vector<UIObject*> UIObject::GetChildren() const {
    std::vector<UIObject*> array;

    for (auto& child : children) {
        array.emplace_back(child.get());
    }

    return array;
}