#include "stdafx.h"
#include "SpriteSheet.h"

TextureRegion SpriteSheet::frame(AssetNameView textureAssetName, int currentFrame, int totalFrame)
{
	const auto texture = TextureAsset(textureAssetName);
	const int frameWidth = texture.width() / totalFrame;
	return texture(Rect{ frameWidth }.movedBy(currentFrame * frameWidth, 0));
}
