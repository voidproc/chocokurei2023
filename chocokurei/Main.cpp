﻿#include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include "TitleScene.h"
#include "MainScene.h"

void Main()
{
	Scene::Resize(SceneSize);
	Scene::SetResizeMode(ResizeMode::Keep);

	int scale = DefaultSceneScale;
	Window::Resize(SceneSize * scale);

	// フォントがぼやけないためのテクスチャフィルタ、レンダーステート設定

	Scene::SetTextureFilter(TextureFilter::Nearest);

	const ScopedRenderStates2D renderState{ SamplerState::ClampNearest };

	Scene::SetBackground(Palette::Black);

	// アセット

	FontAsset::Register(U"main", 12, U"fonts/k8x12L.ttf", FontStyle::Bitmap);
	FontAsset::Register(U"mainb", 12, U"fonts/k8x12L.ttf", FontStyle::BoldBitmap);

	TextureAsset::Register(U"bottomshade", U"images/bottom-shade.png");
	TextureAsset::Register(U"pronamachan", U"images/pronama-chan.png");
	TextureAsset::Register(U"balloon", U"images/balloon.png");

	// シーン

	App manager;
	manager.add<MainScene>(U"MainScene");
	manager.add<TitleScene>(U"TitleScene");
	manager.setFadeColor(Palette::Black);
	manager.changeScene(U"MainScene", 0ms);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
