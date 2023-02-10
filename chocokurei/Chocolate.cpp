#include "stdafx.h"
#include "Chocolate.h"

Chocolate::Chocolate(int type)
	: type_(type)
{
	features_.resize(ChocolateTypesCount);

	//                               name,      heart,square,round, brown,white

	features_[0] = ChocolateFeature{ U"choco0", true, false, false, true, false }; //brown-heart
	features_[1] = ChocolateFeature{ U"choco1", true, false, false, false, true }; //white-heart
	features_[2] = ChocolateFeature{ U"choco2", true, false, false, false, false }; //red-heart
	features_[3] = ChocolateFeature{ U"choco3", true, false, false, false, false }; //purple-heart
	features_[4] = ChocolateFeature{ U"choco4", false, true, false, true, false }; //brown-square
	features_[5] = ChocolateFeature{ U"choco5", false, true, false, false, true }; //white-square
	features_[6] = ChocolateFeature{ U"choco6", false, true, false, true, true }; //white&brown-square(1)
	features_[7] = ChocolateFeature{ U"choco7", false, true, false, true, true }; //white&brown-square(2)
	features_[8] = ChocolateFeature{ U"choco8", false, false, true, false, true }; //white-round
	features_[9] = ChocolateFeature{ U"choco9", false, false, true, true, false }; //brown-round
	features_[10] = ChocolateFeature{ U"choco10", false, false, true, true, false }; //brown-round
	features_[11] = ChocolateFeature{ U"choco11", false, false, true, false, true }; //white-round-face
}

void Chocolate::update()
{
}

void Chocolate::draw() const
{
	TextureAsset(features_[type_].name).drawAt(Scene::Center()); //temp
}
