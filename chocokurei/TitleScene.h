#pragma once

class TitleScene : public App::Scene
{
public:
	TitleScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
};
