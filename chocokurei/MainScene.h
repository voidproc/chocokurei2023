#pragma once

#include "Chocolate.h"
#include "ChocolateBox.h"
#include "Condition.h"
#include "Pronamachan.h"
#include "Balloon.h"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
	int level_ = 1;
	int levelAdjust_ = 0;
	Condition condition_;
	Stopwatch swNextStage_;
	Stopwatch swAnswer_;
	int miss_ = 0;
	Pronamachan pronamachan_;
	Balloon balloon_;
	ChocolateBox chocolateBox_;

	void setNewStage_();
	Condition randomCondition_() const;

	void drawBg_() const;
};
