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
class UIInput;

class UIManager : public ManagerBase {
private:
	std::vector<UICanvasBase*> canvases;

public:
	UIManager() = default;
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
};

#endif // !_UIMANAGER_H_