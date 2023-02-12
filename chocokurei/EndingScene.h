#pragma once

class EndingScene : public App::Scene
{
public:
	EndingScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
	Stopwatch swText_;
	int textLine_ = 0;

	void drawGood_() const;
	void drawBad_() const;
};
