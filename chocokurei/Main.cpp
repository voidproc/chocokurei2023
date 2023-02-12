#include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include "TitleScene.h"
#include "MainScene.h"
#include "EndingScene.h"
#include "const.h"

void loadImages()
{
	TextureAsset::Register(U"balloon", Resource(U"images/balloon.png"));
	TextureAsset::Register(U"bg", Resource(U"images/bg.png"));
	TextureAsset::Register(U"box-3x3", Resource(U"images/box-3x3.png"));
	TextureAsset::Register(U"box-3x4", Resource(U"images/box-3x4.png"));
	TextureAsset::Register(U"box-4x4", Resource(U"images/box-4x4.png"));
	TextureAsset::Register(U"box-4x5", Resource(U"images/box-4x5.png"));
	TextureAsset::Register(U"box-4x6", Resource(U"images/box-4x6.png"));
	TextureAsset::Register(U"choco0", Resource(U"images/choco0.png"));
	TextureAsset::Register(U"choco1", Resource(U"images/choco1.png"));
	TextureAsset::Register(U"choco10", Resource(U"images/choco10.png"));
	TextureAsset::Register(U"choco11", Resource(U"images/choco11.png"));
	TextureAsset::Register(U"choco2", Resource(U"images/choco2.png"));
	TextureAsset::Register(U"choco3", Resource(U"images/choco3.png"));
	TextureAsset::Register(U"choco4", Resource(U"images/choco4.png"));
	TextureAsset::Register(U"choco5", Resource(U"images/choco5.png"));
	TextureAsset::Register(U"choco6", Resource(U"images/choco6.png"));
	TextureAsset::Register(U"choco7", Resource(U"images/choco7.png"));
	TextureAsset::Register(U"choco8", Resource(U"images/choco8.png"));
	TextureAsset::Register(U"choco9", Resource(U"images/choco9.png"));
	TextureAsset::Register(U"good", Resource(U"images/good.png"));
	TextureAsset::Register(U"miss", Resource(U"images/miss.png"));
	TextureAsset::Register(U"next", Resource(U"images/next.png"));
	TextureAsset::Register(U"pronama-chan-fail", Resource(U"images/pronama-chan-fail.png"));
	TextureAsset::Register(U"pronama-chan", Resource(U"images/pronama-chan.png"));
	TextureAsset::Register(U"titlebg", Resource(U"images/titlebg.png"));
}

void Main()
{
	Scene::SetBackground(Palette::Black);

	Window::SetTitle(U"chocokurei2023 {}"_fmt(VersionText));

	Scene::Resize(SceneSize);
	Scene::SetResizeMode(ResizeMode::Keep);

	int scale = DefaultSceneScale;
	Window::Resize(SceneSize * scale);

	Scene::SetTextureFilter(TextureFilter::Nearest);

	const ScopedRenderStates2D renderState{ SamplerState::ClampNearest };

	// アセット

	FontAsset::Register(U"main", 12, Resource(U"fonts/k8x12L.ttf"), FontStyle::Bitmap);
	FontAsset::Register(U"mainb", 12, Resource(U"fonts/k8x12L.ttf"), FontStyle::BoldBitmap);

	loadImages();

	// シーン

	App manager;
	manager.add<TitleScene>(U"TitleScene");
	manager.add<MainScene>(U"MainScene");
	manager.add<EndingScene>(U"EndingScene");

	manager.setFadeColor(Palette::Black);

	manager.changeScene(U"TitleScene", 0ms);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
