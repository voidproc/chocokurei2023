#include "stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene(const InitData& init)
	: IScene{ init }
{
}

void TitleScene::update()
{
	if (swScene_.sF() > 2.0)
	{
		if (MouseL.down() && Scene::Rect().intersects(Cursor::PosF()))
		{
			changeScene(U"MainScene", 0ms);
		}
	}
}

void TitleScene::draw() const
{
	Scene::SetBackground(Color(U"#ffccdf"));

	Rect(Arg::center = Scene::Center().movedBy(0, -8), Size(Scene::Width(), 48)).draw();
	FontAsset(U"mainb")(U"プロ生ちゃんチョコくれい２０２３").drawAt(Scene::Center().movedBy(0, -16), Palette::Brown);
	FontAsset(U"main")(U"Ver.20230214").drawAt(Scene::Center().movedBy(0, 0), Palette::Indianred);

	{
		const ColorF textColor = ColorF(Palette::White, Periodic::Square0_1(700ms));
		FontAsset(U"main")(U"マウスクリックでスタートだよ").drawAt(Scene::Rect().bottomCenter().movedBy(0, -16), textColor);
	}
}
