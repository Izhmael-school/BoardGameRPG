#include "Application.h"
#include "DxLib.h"
// #include "EffekseerForDXLib.h"
#include <ioStream>
#include <random>
#include <memory>
#include "Manager/Input/InputManager.h"
#include "Manager/Fade/FadeManager.h"
#include "Manager/Scene/SceneManager.h"
#include "Manager/Time/TimeManager.h"
#include "Manager/Resource/Model/ModelResourceManager.h"
#include "Manager/Resource/Effect/EffectResourceManager.h"
#include "Manager/Resource/Audio/AudioResourceManager.h"
#include "ImGui.h"

Application::~Application() = default;
Application::Application()
	: isGameEnd(true)
{}

int Application::Init() {
	// ImGui縺ｮ蛻晄悄蛹・
	imgui.Init();

	// 荵ｱ謨ｰ隱ｿ遽
	std::random_device rd;
	std::mt19937_64 mt(rd());
	SRand(static_cast<int>(mt()));

	return 0;
}

int Application::DxLibInit() {
#pragma region // DxLib縺ｮ蛻晄悄蛹門・逅・隗ｦ繧九∋縺九ｉ縺・
	// 繧ｿ繧､繝医Ν縺ｮ螟画峩
	SetWindowText("ExHand");
	// XInput蟇ｾ蠢懊ご繝ｼ繝繝代ャ繝芽ｨｭ螳・
	SetUseXInputFlag(true);
	// 繧ｦ繧｣繝ｳ繝峨え縺ｮ繧ｵ繧､繧ｺ繧貞､画峩縺吶ｋ
	SetGraphMode(1920, 1080, 32, FPS);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
	SetFullSceneAntiAliasingMode(4, 2);
	// 繧ｲ繝ｼ繝繧｢繧､繧ｳ繝ｳ
	SetWindowIconID(101);
	// 文字コードをUTF-8に変更
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	// 繝ｭ繧ｰ繝輔ぃ繧､繝ｫ繧呈ｮ九＆縺ｪ縺・
#if _DEBUG
	SetOutApplicationLogValidFlag(TRUE);
#else
	SetOutApplicationLogValidFlag(FALSE);
#endif

	// 襍ｷ蜍墓凾縺ｮ繧ｦ繧｣繝ｳ繝峨え縺ｮ繝｢繝ｼ繝峨・險ｭ螳・
#if _DEBUG
	ChangeWindowMode(TRUE);	// TRUE : 繧ｦ繧｣繝ｳ繝峨え繝｢繝ｼ繝・FALSE : 繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ
#else
	ChangeWindowMode(FALSE);	// TRUE : 繧ｦ繧｣繝ｳ繝峨え繝｢繝ｼ繝・FALSE : 繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ
#endif

	// 閭梧勹濶ｲ縺ｮ險ｭ螳・
#if _DEBUG
	SetBackgroundColor(196, 196, 196);
#else 
	SetBackgroundColor(196, 196, 196);
#endif

	// Dxlib縺ｮ蛻晄悄蛹・
	if (DxLib_Init() == -1)
		return 1;

	/*if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 1;
	}*/

	// 謠冗判縺吶ｋ蜈医ｒ險ｭ螳壹☆繧・陬冗判髱｢縺ｫ螟画峩縺吶ｋ
	SetDrawScreen(DX_SCREEN_BACK);

	// 蝗ｳ蠖｢謠冗判縺ｮZ繝舌ャ繝輔ぃ縺ｮ譛牙柑蛹・
	{
		// Z繝舌ャ繝輔ぃ繧剃ｽｿ逕ｨ縺吶ｋ縺九←縺・°
		SetUseZBuffer3D(TRUE);	// default : FALSE
		// Z繝舌ャ繝輔ぃ縺ｫ譖ｸ縺崎ｾｼ縺ｿ繧定｡後≧縺・
		SetWriteZBuffer3D(TRUE); // default : FALSE
	}
	int light = CreateDirLightHandle(VGet(0.0f, 1.0f, 0.0f));

	// 繝ｩ繧､繝・ぅ繝ｳ繧ｰ
	{
		// 繝ｩ繧､繝医・險育ｮ励ｒ縺ｩ縺・☆繧九°
		SetUseLighting(FALSE); // default : TRUE
		// 讓呎ｺ悶Λ繧､繝医ｒ菴ｿ逕ｨ縺吶ｋ縺九←縺・°
		SetLightEnable(FALSE);	// default : TRUE
		SetLightDifColorHandle(light, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		SetLightSpcColorHandle(light, GetColorF(0.0f, 0.0f, 0.0f, 1.0f)); // 繧ｹ繝壹く繝･繝ｩ縺ｪ縺・
		SetLightEnableHandle(light, TRUE);
		SetLightPositionHandle(light, VGet(0, 10000, 0));
		// 繧ｰ繝ｭ繝ｼ繝舌Ν迺ｰ蠅・・縺ｮ險ｭ螳・
		SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.5f));
		//// 蜿榊ｰ・・縺ｮ險ｭ螳・ Diffuse
		//SetLightDifColor(GetColorF(1, 0, 0, 0));
		//// 髀｡髱｢蜿榊ｰ・・縺ｮ險ｭ螳壹Specular
		//SetLightSpcColor(GetColorF(1, 0, 0.25f, 1));
		//// 迺ｰ蠅・・縺ｮ險ｭ螳壹Ambient
		//SetLightAmbColor(GetColorF(1, 1, 1, 1));
	}
#pragma endregion

    return 0;
}

bool Application::Update() {
	// 邂｡逅・け繝ｩ繧ｹ縺ｮ譖ｴ譁ｰ
	TimeManager::GetInstance().Update(0.0f);
	float t = TimeManager::GetInstance().GetDeltaTime();
	InputManager::GetInstance().Update(t);
	SceneManager::GetInstance().Update(t);
	FadeManager::GetInstance().Update(t);

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE)) {
		return true;
	}

	return false;
}

void Application::Render() {
	printfDx("%f\n", GetFPS());
	SceneManager::GetInstance().Render();
	FadeManager::GetInstance().Render();

	ImGui::Begin("ResourceCounter");
	ImGui::Text("Model:%d\n", modelResourceManager->GetModelNum());
	ImGui::Text("Effect:%d\n", effectResourceManager->GetEffectResourceCount());
	ImGui::Text("Audio:%d\n", audioResourceManager->GetAudioResourceCount());
	ImGui::End();
}

void Application::ResourceLoad() {
	// インスタンスの作成
	modelResourceManager = std::make_unique<ModelResourceManager>();
	effectResourceManager = std::make_unique<EffectResourceManager>();
	audioResourceManager = std::make_unique<AudioResourceManager>();
	// ロード
	modelResourceManager->LoadModelFromExternalFile();
	effectResourceManager->LoadEffectFromExternalFile();
	audioResourceManager->LoadAudioFromExternalFile();
}

void Application::ResourceDelete() {
	DeleteLightHandleAll();
	MV1InitModel();
	InitSoundMem();
	InitGraph();
	InitFontToHandle();
}

void Application::End() {
	// ImGui縺ｮ邨ゆｺ・・逅・
	imgui.Release();
}

void Application::DxLibEnd() {
	// DxLib縺ｮ邨ゆｺ・
	//Effkseer_End();
	DxLib_End();
}

void Application::Run() {
	// 蛻晄悄蛹・
	int dxLibInitComplete = DxLibInit();
	int initComplete = Init();
	// 蛻晄悄蛹悶↓螟ｱ謨励＠縺溘ｉ繧ｲ繝ｼ繝繧堤ｵゆｺ・☆繧・
	isGameEnd = initComplete | dxLibInitComplete;

	// リソースの読み込み
	ResourceLoad();

	// 繝｡繧､繝ｳ繝ｫ繝ｼ繝・
	while (ProcessMessage() == 0) {
		// 邨ゆｺ・
		if (isGameEnd)break;
		// 繝輔Ξ繝ｼ繝髢句ｧ区凾蛻ｻ繧貞叙蠕・
		int frameStart = GetNowCount();
		// 逕ｻ髱｢繧偵け繝ｪ繧｢縺吶ｋ
		ClearDrawScreen();
		// ImGui縺ｮ繝輔Ξ繝ｼ繝蛻昴ａ縺ｫ蜻ｼ縺ｶ蜃ｦ逅・
		imgui.BeginFrame();
		// 譖ｴ譁ｰ
		isGameEnd = Update();
		// 謠冗判
		Render();
		// ImGui縺ｮ繝輔Ξ繝ｼ繝邨ゅｏ繧翫↓蜻ｼ縺ｶ蜃ｦ逅・
		imgui.EndFrame();
		// 陬冗判髱｢縺ｨ陦ｨ逕ｻ髱｢繧貞・繧頑崛縺医ｋ
		ScreenFlip();
		// Debug繝ｭ繧ｰ繧ｯ繝ｪ繧｢
		clsDx();

		// 蜃ｦ逅・↓縺九°縺｣縺滓凾髢薙ｒ險育ｮ・
		int elapsed = GetNowCount() - frameStart;
		int update = int(FRAME_TIME * 1000.0f);
		// 蜃ｦ逅・′騾溘☆縺弱◆繧牙ｾ・▽
		if (elapsed < update)
			WaitTimer(update - elapsed);
	}
	// 邨ゆｺ・燕蜃ｦ逅・
	ResourceDelete();
	End();
	DxLibEnd();
}
