/*
 * @brief 各UIキャンバスの管理をする
 * @author Sekino
 */
#pragma once
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "../ManagerBase.h"
#include <memory>
#include <vector>

class UICanvasBase;
class UIContext;
struct UIInput;

class UIManager : public ManagerBase {
private:
	std::vector<UICanvasBase*> canvases;
	std::shared_ptr<UIContext> context;
	int text = 5;
public:
	UIManager();
	~UIManager() = default;

	void Update(float _t,UIInput& _input) ;

	void Render() override;

	void PushCanvas(UICanvasBase* _pCanvas);

	/*
	 * @brief 最前のものを消す
	 */
	void PopCanvas();

	/*
	 * @brief 引数と一致するものを消す
	 */
	void PopCanvas(UICanvasBase* _pCanvas);

	/*
	 * @brief 変数参照用のcontextを返す
	 */
	std::shared_ptr<UIContext> GetContext() const { return context; }
};

#endif // !_UIMANAGER_H_