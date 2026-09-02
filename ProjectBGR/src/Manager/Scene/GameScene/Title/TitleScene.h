/*
 *	@file	TitleScene.h
 *  @author Sekino
 */

#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "../SceneBase.h"

/*
 *	タイトルシーン
 */
class TitleScene : public SceneBase {
private:

public:
	/*
	 *	コンストラクタ
	 */
	TitleScene();
	/*
	 *	デストラクタ
	 */
	~TitleScene();

private:
	void Start() override;

public:
	void Update(float _t) override;

	void Render() override;

	void Setup() override;

	void Cleanup() override;
};

#endif // !_TITLE_SCENE_H_