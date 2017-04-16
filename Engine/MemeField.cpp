#include "MemeField.h"
#include "SpriteCodex.h"
#include <assert.h>
#include <random>

MemeField::MemeField(int nMemes):
	pos(100,100)
{
	assert(nMemes > 0 && nMemes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width);
	std::uniform_int_distribution<int> yDist(0, height);
	
	for (int i = 0; i < nMemes; i++) {
		const Vei2 gridPos = { xDist(rng), yDist(rng) };
		if (!field[gridPos.y * width + gridPos.x].HasMeme()) {
			field[gridPos.y * width + gridPos.x].SpawnMeme();
		}
	}
}

void MemeField::Draw(Graphics & gfx) const
{
	const RectI rect = RectI(pos, width*SpriteCodex::tileSize, height*SpriteCodex::tileSize);
	gfx.DrawRect(rect, SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++) {
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
			field[gridPos.y * width + gridPos.x].Draw(pos + gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

void MemeField::OnRevealedClick(const Vei2 screenPos)
{
	Reveal(ScreenToGrid(screenPos-pos));
}

void MemeField::Reveal(const Vei2 gridPos)
{
	field[gridPos.y * width + gridPos.x].Reveal();
}

Vei2 & MemeField::ScreenToGrid(Vei2 screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

RectI MemeField::GetRect() const
{
	// TODO: insert return statement here
	return RectI(pos, width*SpriteCodex::tileSize, height*SpriteCodex::tileSize);
}

void MemeField::Tile::SpawnMeme()
{
	assert(hasMeme == false);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2 & screenPos, Graphics & gfx) const
{
	assert(screenPos.x >= 0 && screenPos.x < Graphics::ScreenWidth - SpriteCodex::tileSize &&
		screenPos.y >= 0 && screenPos.y < Graphics::ScreenHeight - SpriteCodex::tileSize);
	switch (state) {
	case Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case Revealed:
		if (hasMeme) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else {
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		break;
	default:
		assert(false);
	}
}

void MemeField::Tile::Reveal()
{
	state = State::Revealed;
}
