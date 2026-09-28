/*
 * @file SceneBase.cpp
 * @author Sekino
 */
#include "SceneBase.h"
#include "UI/Canvas/UICanvasBase.h"

SceneBase::SceneBase() 
	:sceneCanvas(nullptr)
	{ Start(); }

SceneBase::~SceneBase() = default;

void SceneBase::Start(){}

void SceneBase::Setup(){}

void SceneBase::Cleanup(){}
