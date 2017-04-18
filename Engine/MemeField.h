#pragma once

#include "Graphics.h"
#include "Vei2.h"

class MemeField {
private:
	class Tile {
	public:
		enum State {
			Hidden,
			Flagged,
			Revealed,
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, bool& isGameOver, Graphics& gfx) const;
		void Reveal();
		void ToggleFlag(int &correctFlagged);
		bool IsEmpty();
		bool IsHidden();
	public:
		int nearMemeCount = 0;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx);
	bool OnRevealedClick(const Vei2 screenPos);
	void Reveal(const Vei2 gridPos);
	bool OnFlagClick(const Vei2 screenPos);
	void ToggleFlag(const Vei2 gridPos);
	Vei2& ScreenToGrid(const Vei2 screenPos);
	RectI GetRect() const;
private:
	static constexpr int width = 24;
	static constexpr int height = 16;
	Vei2 pos;
	Tile field[width * height];
	bool isGameOver = false;
	class Game *game;
	int correctFlagged = 0;
	int nMemes;
	int revealCount = 0;
};
