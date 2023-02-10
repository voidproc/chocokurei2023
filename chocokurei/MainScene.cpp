#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene(const InitData& init)
	: IScene{ init }, choco_{}
{
	for (int i : step(9))
	{
		choco_.emplace_back<Chocolate>(1);

		const int x = (i % 3) * 20 + 24;
		const int y = (i / 3) * 20 + 24;
		choco_.back().moveTo(Vec2(x, y), 0.8 + Random(-0.2, 0.2));
	}
}

void MainScene::update()
{
	const Array<String> condMsg = {
		U"すきなチョコ",
		U"ハート型のチョコ",
		U"まるいチョコ",
		U"四角いチョコ",
		U"茶色のチョコ",
		U"ホワイトチョコ",
		U"一番多いの",
		U"一番少ないの",
	};

	const Array<String> endMsg = {
		U"を取っていいよ",
		U"を食べていいよ",
		U"をあげる",
		U"をどうぞ",
		U"を取ってね",
		U"を食べてね",
	};

	if (msg_.empty() || swMsgRefresh_.sF() > 5.0)
	{
		msg_ = condMsg.choice() + endMsg.choice();

		swMsgRefresh_.restart();
	}

	for (auto& choco : choco_)
	{
		choco.update();
	}
}

void MainScene::draw() const
{
	Scene::SetBackground(Color(U"#dc678c"));

	{
		// 基準点
		const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

		// 影
		TextureAsset(U"bottomshade").draw(basePos);

		// プロ生ちゃん
		TextureAsset(U"pronamachan").draw(basePos);

		// 吹き出し
		TextureAsset(U"balloon").draw(basePos);

		FontAsset(U"main")(msg_).draw(basePos.movedBy(42, 12), Palette::Black);
	}

	for (const auto& choco : choco_)
	{
		choco.draw();
	}
}
