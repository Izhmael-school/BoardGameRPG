/*
 *	@file	TitleScene.h
 *  @author Sekino
 */

#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "../SceneBase.h"

/*
 *	繧ｿ繧､繝医Ν繧ｷ繝ｼ繝ｳ
 */
class TitleScene : public SceneBase {
private:

public:
	/*
	 *	繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	 */
	TitleScene();
	/*
	 *	繝・せ繝医Λ繧ｯ繧ｿ
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