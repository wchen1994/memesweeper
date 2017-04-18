#include "MemeField.h"
#include "SpriteCodex.h"
#include "Game.h"
#include <assert.h>
#include <random>
#include <algorithm>

MemeField::MemeField(int nMemes):
	pos(100,100)
{
	assert(nMemes > 0 && nMemes < width * height);
	this->nMemes = nMemes;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width-1);
	std::uniform_int_distribution<int> yDist(0, height-1);
	
	for (int i = 0; i < nMemes; i++) {
		const Vei2 gridPos = { xDist(rng), yDist(rng) };
		if (!field[gridPos.y * width + gridPos.x].HasMeme()) {
			field[gridPos.y * width + gridPos.x].SpawnMeme();
			field[gridPos.y * width + gridPos.x].nearMemeCount = 0;
			//increment the memeCount on tile around
			for (int y = std::max(0, gridPos.y - 1); y <= std::min(height - 1, gridPos.y + 1); y++) {
				for (int x = std::max(0, gridPos.x - 1); x <= std::min(width - 1, gridPos.x + 1); x++) {
					Tile& tile = field[y*width + x];
					if (!tile.HasMeme()) {
						tile.nearMemeCount++;
					}
				}
			}
		}
	}
}

void MemeField::Draw(Graphics & gfx)
{
	const RectI rect = RectI(pos, width*SpriteCodex::tileSize, height*SpriteCodex::tileSize);
	gfx.DrawRect(rect, SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++) {
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
			field[gridPos.y * width + gridPos.x].Draw(pos + gridPos * SpriteCodex::tileSize, isGameOver, gfx);
		}
	}
}

bool MemeField::OnRevealedClick(const Vei2 screenPos)
{
	Reveal(ScreenToGrid(screenPos-pos));
	if (isGameOver) {
		return true;
	}
	return false;
}

void MemeField::Reveal(const Vei2 gridPos)
{
	Tile& tile = field[gridPos.y * width + gridPos.x];
	tile.Reveal();
	revealCount++;
	if (tile.HasMeme()||revealCount == width * height - nMemes) {
		isGameOver = true;
	}
	if (tile.IsEmpty()) {
		for (int y = std::max(0, gridPos.y - 1); y <= std::min(height-1, gridPos.y + 1); y++) {
			for (int x = std::max(0, gridPos.x - 1); x <= std::min(width-1, gridPos.x + 1); x++) {
				if (field[y*width + x].IsHidden()) {
					Reveal(Vei2(x, y));
				}
			}
		}
	}
}

bool MemeField::OnFlagClick(const Vei2 screenPos)
{
	ToggleFlag(ScreenToGrid(screenPos-pos));
	if (isGameOver) {
		return true;
	}
	return false;
}

void MemeField::ToggleFlag(const Vei2 gridPos)
{
	field[gridPos.y * width + gridPos.x].ToggleFlag(correctFlagged);
	if (correctFlagged == nMemes) {
		isGameOver = true;
	}
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

void MemeField::Tile::Draw(const Vei2 & screenPos, bool& isGameOver, Graphics & gfx) const
{
	assert(screenPos.x >= 0 && screenPos.x < Graphics::ScreenWidth - SpriteCodex::tileSize &&
		screenPos.y >= 0 && screenPos.y < Graphics::ScreenHeight - SpriteCodex::tileSize);
	switch (state) {
	case Hidden:
		if (isGameOver && hasMeme) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else if (isGameOver) {
			switch (nearMemeCount) {
			case 0:
				SpriteCodex::DrawTile0(screenPos, gfx);
				break;
			case 1:
				SpriteCodex::DrawTile1(screenPos, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile2(screenPos, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile3(screenPos, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile4(screenPos, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile5(screenPos, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile6(screenPos, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile7(screenPos, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile8(screenPos, gfx);
				break;
			}
		}
		else {
			SpriteCodex::DrawTileButton(screenPos, gfx);
		}
		break;
	case Flagged:
		if (isGameOver && !hasMeme) {
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			SpriteCodex::DrawTileCross(screenPos, gfx);
		}
		else if (isGameOver) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
		}
		else {
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
		}
		break;
	case Revealed:
		if (hasMeme) {
			if (isGameOver && hasMeme) {
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			else {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
		}
		else {
			switch (nearMemeCount) {
			case 0:
				SpriteCodex::DrawTile0(screenPos, gfx);
				break;
			case 1:
				SpriteCodex::DrawTile1(screenPos, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile2(screenPos, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile3(screenPos, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile4(screenPos, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile5(screenPos, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile6(screenPos, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile7(screenPos, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile8(screenPos, gfx);
				break;
			}
			
		}
		break;
	default:
		assert(false);
	}
}

void MemeField::Tile::Reveal()
{
	if (state != State::Flagged) {
		state = State::Revealed;
	}
}

void MemeField::Tile::ToggleFlag(int& correctFlagged)
{
	if (state == State::Hidden) {
		state = State::Flagged;
		if (hasMeme) {
			correctFlagged++;
		}
	}
	else if (state == State::Flagged){
		state = State::Hidden;
		if (hasMeme) {
			correctFlagged--;
		}
	}
}

bool MemeField::Tile::IsEmpty()
{
	return nearMemeCount == 0 && !hasMeme;
}

bool MemeField::Tile::IsHidden()
{
	return state == State::Hidden;
}
