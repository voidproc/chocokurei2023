#pragma once

#include "Chocolate.h"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };

	Stopwatch swMsgRefresh_{ StartImmediately::Yes };

	String msg_;

	Array<Chocolate> choco_;
};
