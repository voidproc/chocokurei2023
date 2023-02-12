#include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include "TitleScene.h"
#include "MainScene.h"
#include "EndingScene.h"
#include "const.h"

void loadImages()
{
	for (const auto& file : FileSystem::DirectoryContents(U"images", Recursive::No))
	{
		if (FileSystem::Extension(file) == U"png")
		{
			TextureAsset::Register(FileSystem::BaseName(file), file);
		}
	}
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

	FontAsset::Register(U"main", 12, U"fonts/k8x12L.ttf", FontStyle::Bitmap);
	FontAsset::Register(U"mainb", 12, U"fonts/k8x12L.ttf", FontStyle::BoldBitmap);

	loadImages();

	// シーン

	App manager;
	manager.add<TitleScene>(U"TitleScene");
	manager.add<MainScene>(U"MainScene");
	manager.add<EndingScene>(U"EndingScene");

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
