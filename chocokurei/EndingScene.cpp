#include "stdafx.h"
#include "EndingScene.h"
#include "Constants.h"

EndingScene::EndingScene(const InitData& init)
	: IScene{ init }
{
}

void EndingScene::update()
{
	if (not swText_.isRunning() && swScene_.sF() > 2.0)
	{
		swText_.restart();
	}

	const int textLines = EndingText[static_cast<int>(getData().endingType)].size();

	if (swText_.sF() > 2.5 && Scene::Rect().leftClicked())
	{
		if (textLine_ < textLines - 1)
		{
			textLine_++;
			swText_.restart();
		}
	}

	if (swText_.sF() > 5.0 && Scene::Rect().leftClicked())
	{
		if (textLine_ == textLines - 1)
		{
			changeScene(U"TitleScene", 0s);
		}
	}
}

void EndingScene::draw() const
{
	if (getData().endingType == EndingType::Good)
	{
		drawGood_();
	}
	else
	{
		drawBad_();
	}
}

void EndingScene::drawGood_() const
{
	const Array<String>& endingText = EndingText[static_cast<int>(EndingType::Good)];

	Scene::SetBackground(ColorF{ U"#d77bba" });

	const Color textColor = ColorF{ 1 };
	auto text = endingText[textLine_].substrView(0, (int)(32 * swText_.sF()));
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }.movedBy(1, 1), ColorF{ Palette::Black, 0.2 });
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }, textColor);

	if (swText_.sF() > 2.5 && textLine_ < endingText.size() - 1)
	{
		TextureAsset(U"next").draw(Scene::Width() - 24, Scene::CenterF().y + 8, ColorF{ 1, Periodic::Square0_1(0.5s) });
	}

	FontAsset(U"main")(U"プレイしてくれてありがとう!\nこのゲームはvoidProcが2023年に作りました.")
		.drawAt(Scene::Rect().bottomCenter().movedBy(0, -18), ColorF{1, 0.5});
}

void EndingScene::drawBad_() const
{
	const Array<String>& endingText = EndingText[static_cast<int>(EndingType::Bad)];

	Scene::SetBackground(ColorF{ 0 });

	const Color textColor = ColorF{ 1 };
	auto text = endingText[textLine_].substrView(0, (int)(32 * swText_.sF()));
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }.movedBy(1, 1), ColorF{ Palette::White, 0.3 });
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2}, textColor);

	if (swText_.sF() > 2.5 && textLine_ < endingText.size() - 1)
	{
		TextureAsset(U"next").draw(Scene::Width() - 24, Scene::CenterF().y + 16, ColorF{ 1, Periodic::Square0_1(0.5s)});
	}
}
