#pragma once
#include "Graphics.h"

class MemeField {
private:
	class Tile {
	private:
		enum class State {
			Hidden,
			Flagged,
			Revealed
		};
		State state = State::Hidden;
		bool hasMeme = false;
	public:
		void Draw( const Vei2& screenPos , Graphics& gfx) const;
		void SpawnMeme();
		bool HasMeme()const;
	};


public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx);
	RectI GetRect()const;

private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos)const ;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile Field[width * height];

};