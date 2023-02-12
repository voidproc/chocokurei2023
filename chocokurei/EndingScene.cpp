#include "stdafx.h"
#include "EndingScene.h"

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

	if (swText_.sF() > 2.5 && Scene::Rect().leftClicked())
	{
		if (textLine_ < (getData().endingType == EndingType::Good ? 6 : 5) - 1)
		{
			textLine_++;
			swText_.restart();
		}
	}

	if (swText_.sF() > 5.0 && textLine_ == ((getData().endingType == EndingType::Good ? 6 : 5) - 1) && Scene::Rect().leftClicked())
	{
		changeScene(U"TitleScene", 0s);
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
	const Array<String> endingText = {
		U"「どう？ この日のために、たくさん用意したんだよ！」",
		U"プロ生ちゃんから、きれいに包装されたチョコの詰め合わせを受け取った。",
		U"どうやらプロ生ちゃん手作りのチョコらしい。甘くてほろ苦くて、とてもおいしいと感じる。",
		U"「遠慮しないで、どんどん食べて！ おいしい？」",
		U"プロ生ちゃんに促されるが、うまく手が動かない。プロ生ちゃんが、笑顔でこちらを見ている。",
		U"これは現実ではないのか？ 視界が白い霧に包まれていく中で、この夢が少しでも長く続いてくれと願うのだった。\n- Good End -",
	};

	const Color bgColor = ColorF{ U"#d77bba" };
	const Color textColor = ColorF{ 1 };

	Scene::SetBackground(bgColor);

	Scene::SetBackground(bgColor);

	auto text = endingText[textLine_].substrView(0, (int)(32 * swText_.sF()));
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }.movedBy(1, 1), ColorF{ Palette::Black, 0.2 });
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }, textColor);

	if (swText_.sF() > 2.5 && textLine_ < 6 - 1)
	{
		TextureAsset(U"next").draw(Scene::Width() - 24, Scene::CenterF().y + 8, ColorF{ 1, Periodic::Square0_1(0.5s) });
	}

	FontAsset(U"main")(U"プレイしてくれてありがとう!\nこのゲームはvoidProcが2023年に作りました.")
		.drawAt(Scene::Rect().bottomCenter().movedBy(0, -18), ColorF{1, 0.5});

}

void EndingScene::drawBad_() const
{
	const Array<String> endingText = {
		U"イベント会場でプロ生ちゃんの機嫌を損ねてしまった俺は、スタッフに腕を掴まれ会場の外へつまみ出された。",
		U"プロ生ちゃんの冷たい視線が目に焼き付いている。なぜ、こんなことになってしまったのか。",
		U"俺はただ、プロ生ちゃんからチョコをたくさんもらいたかっただけなのに……",
		U"しかし、代償は大きかった。\n――――今後、イベントへの出入り禁止を言い渡された俺は、",
		U"Twitterアカウントを削除し、その後一切プロ生ちゃんと関わることはなかった。\n- Bad End -",
	};

	const Color bgColor = ColorF{ 0 };
	const Color textColor = ColorF{ 1 };

	Scene::SetBackground(bgColor);

	auto text = endingText[textLine_].substrView(0, (int)(32 * swText_.sF()));
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2 }.movedBy(1, 1), ColorF{ Palette::White, 0.3 });
	FontAsset(U"main")(text).draw(RectF{ 8, Scene::CenterF().y - 24, Scene::Width() - 8 * 2, 24 * 2}, textColor);

	if (swText_.sF() > 2.5 && textLine_ < 5 - 1)
	{
		TextureAsset(U"next").draw(Scene::Width() - 24, Scene::CenterF().y + 16, ColorF{ 1, Periodic::Square0_1(0.5s)});
	}
}
