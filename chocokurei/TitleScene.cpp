#include "stdafx.h"
#include "TitleScene.h"
#include "const.h"

TitleScene::TitleScene(const InitData& init)
	: IScene{ init }, scale_(DefaultSceneScale)
{
}

void TitleScene::update()
{
	if (swScene_.sF() > 1.0)
	{
		if (Scene::Rect().leftClicked() && not swFade_.isRunning())
		{
			swFade_.restart();
		}

		if (KeyUp.down())
		{
			scale_ = Clamp(scale_ + 1, 2, 8);
			Window::Resize(SceneSize * scale_);
		}

		if (KeyDown.down())
		{
			scale_ = Clamp(scale_ - 1, 2, 8);
			Window::Resize(SceneSize * scale_);
		}
	}

	if (swFade_.isRunning() && swFade_.sF() >= 1.5)
	{
		swFade_.pause();
		changeScene(U"MainScene", 0ms);
	}
}

void TitleScene::draw() const
{
	Scene::SetBackground(Color(U"#ffccdf"));

	TextureAsset(U"titlebg").draw();

	const auto region = FontAsset(U"mainb")(TitleText).regionAt(Scene::Center().movedBy(0, -8));
	region.stretched(8, -3).rounded(3).draw(ColorF(1, 0.5));

	FontAsset(U"mainb")(TitleText).drawAt(Scene::Center().movedBy(0, -8+1), ColorF(Palette::Red, 0.2));
	FontAsset(U"mainb")(TitleText).drawAt(Scene::Center().movedBy(0, -8), Palette::Brown);
	FontAsset(U"main")(VersionText).drawAt(Scene::Center().movedBy(0, 8), Palette::Indianred);

	if (swScene_.sF() > 1.0)
	{
		const ColorF textColor = ColorF(Palette::Brown, 0.5 + 0.5 * Periodic::Square0_1(1s));
		const Array<String> text = {
			U"マウスクリックでスタートだよ",
			U"↑・↓キーで画面の大きさを変えられるよ",
		};
		FontAsset(U"main")(text[1-(int)Periodic::Square0_1(4s)]).drawAt(Scene::Rect().bottomCenter().movedBy(0, -32), textColor);
	}

	// フェード

	if (swFade_.isRunning())
	{
		const double t = swFade_.sF() / 1.0;
		const double a = Min(1.0, 1.0 * EaseOutCubic(t));
		Scene::Rect().draw(ColorF(0, a));
	}

}
