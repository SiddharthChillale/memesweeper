#pragma once
#include "Graphics.h"
#include "Mouse.h"
#include "SpriteCodex.h"

class MemeField {
private:
	class Tile {
	public:
		enum class State {
			Hidden,
			Flagged,
			Revealed
		};
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int memeNeighbours;
	public:
		void Draw( const Vei2& screenPos , Graphics& gfx) const;
		void DrawCaught(const Vei2& screenPos, Graphics& gfx) const;

		
		void UpdateMemesNearby(int memeCount) ;
		//void SetState(const State in_state);
		void SpawnMeme(); // why public
		int GetMemeCount()const;
		void Reveal();
		bool IsRevealed()const;
		void Flag(bool flag_state);
		
		bool IsFlagged()const;
		bool HasMeme()const;
		
	};


public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx);
	RectI GetRect()const;
	bool UpdateOnClick(const Vei2& screenPos);
	void FlagOnCLick(const Vei2& screenPos);
	void RevealEmptyNeighbours(Vei2& gridPos);
	void SetGameOver();
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos)const ;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	static constexpr int left = (Graphics::ScreenWidth - width * SpriteCodex::tileSize) / 2;
	static constexpr int top = 50;
	Vei2 topleft = { left, top };
	Tile Field[width * height];
	bool memeStepped = false;
	Vei2 caught_meme_trap;
};