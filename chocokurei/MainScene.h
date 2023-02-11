#pragma once

#include "Chocolate.h"
#include "Condition.h"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
	Stopwatch swBalloonTextAnimate_;
	String balloonText_;
	Array<Chocolate> choco_;
	int level_ = 0;
	int column_ = 3;
	int row_ = 3;
	Condition condition_;

	void setNewStage_();
	void setBalloonText_(StringView text);
	void drawPronamachan_() const;
	void drawBalloon_() const;
	bool isFullFilledCondition_(const Chocolate& target, Condition cond);

};
