#include "MemeField.h"
#include "SpriteCodex.h"
#include <assert.h>
#include <random>
#include <algorithm>

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
    
    switch (state){
    case State::Hidden:
        SpriteCodex::DrawTileButton(screenPos, gfx);
        break;
    case State::Flagged:
        SpriteCodex::DrawTileButton(screenPos, gfx);
        SpriteCodex::DrawTileFlag(screenPos, gfx);
        break;
    case State::Revealed: {
        if (!hasMeme) {
            switch (memeNeighbours) {
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
            SpriteCodex::DrawTileBomb(screenPos, gfx);
        }
        break;
    }

    }
}
void MemeField::Tile::DrawCaught(const Vei2& screenPos, Graphics& gfx) const
{
    SpriteCodex::DrawTileBombRed(screenPos, gfx);
}


void MemeField::Tile::UpdateMemesNearby(int memeCount)
{
    // code for checking num of memes nearby
    // set number for each tile in its object
    assert(memeCount >= 0 && memeCount < 9);
    memeNeighbours = memeCount;


}



void MemeField::Tile::SpawnMeme()
{
    assert(!hasMeme);
    hasMeme = true;
}
int MemeField::Tile::GetMemeCount() const
{
    return memeNeighbours;
}
void MemeField::Tile::Reveal()
{
    state = State::Revealed;
    

}
bool MemeField::Tile::IsRevealed() const
{
    return state == State::Revealed;
}
void MemeField::Tile::Flag(bool flag_state)
{
    
    if (flag_state) {
        state = State::Flagged;
    }
    else {
        state = State::Hidden;
    }
}

bool MemeField::Tile::IsFlagged() const
{
    return state == State::Flagged;
}
bool MemeField::Tile::HasMeme() const
{
    return hasMeme;
}





MemeField::MemeField(int nMemes)
{
    assert(nMemes > 0 && nMemes < width* height);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    for (int i = 0; i < nMemes; i++) {
        Vei2 spawnPos;

        do {

            spawnPos = { xDist(rng), yDist(rng) };

        } while (TileAt(spawnPos).HasMeme());

        TileAt(spawnPos).SpawnMeme();
    }

    for (Vei2 gridPos = { 0, 0}; gridPos.y < height; gridPos.y++) {
        for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {


            int memeCount = 0;
            int x_Start = std::max(0, gridPos.x - 1);
            int y_Start = std::max(0, gridPos.y - 1);

            int x_End = std::min(width, gridPos.x + 1);            
            int y_End = std::min(height, gridPos.y + 1);

            for (int xi=  x_Start; xi< x_End; xi++) {
                for (int yi = y_Start; yi < y_End; yi++) {
                    Tile& tile = TileAt(Vei2(xi, yi));
                    if (tile.HasMeme()) {
                        memeCount++;
                    }
                }
            }

            TileAt(gridPos).UpdateMemesNearby(memeCount);
        }
    }
    
    
}



void MemeField::Draw(Graphics& gfx)
{
    // show all memes if stepped on one


    gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
    for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; gridPos.y++) {
        for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {

            TileAt(gridPos).Draw(topleft + gridPos * SpriteCodex::tileSize, gfx);
        }
    }
    if (memeStepped) {
        TileAt(caught_meme_trap).DrawCaught(topleft + caught_meme_trap * SpriteCodex::tileSize, gfx);
    }
}

RectI MemeField::GetRect() const
{
    return RectI(left, left + width * SpriteCodex::tileSize, top, top + height * SpriteCodex::tileSize);
}

bool MemeField::UpdateOnClick(const Vei2& screenPos)
{

    
    Vei2 gridPos = (screenPos - topleft) / SpriteCodex::tileSize;
    assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);

    Tile& interestedTile = TileAt(gridPos);

    if (!interestedTile.IsRevealed() && !interestedTile.IsFlagged() ) {
        if (interestedTile.GetMemeCount() == 0) {
            RevealEmptyNeighbours(gridPos);
        }

        interestedTile.Reveal();
        // insert recursion here for revealing all nearby 0 neighbours tiles
    }

    if (interestedTile.HasMeme()) {

        SetGameOver();
        caught_meme_trap = gridPos;
        return true;

    }

    return false;

    
}




void MemeField::RevealEmptyNeighbours(Vei2& gridPos)
{

    int x1_limit = std::max(0, gridPos.x - 1);
    int x2_limit = std::min(width, gridPos.x + 1);

    int y1_limit = std::max(0, gridPos.y - 1);
    int y2_limit = std::min(height, gridPos.y + 1);

    for (int xi = x1_limit; xi <= x2_limit ; xi++) {
        for (int yi = y1_limit; yi <= y2_limit ; yi++) {
            Tile& curiousTile = TileAt(Vei2(xi, yi));

            
            if (!curiousTile.IsRevealed() && !curiousTile.HasMeme()) {
                
                if (curiousTile.GetMemeCount() == 0) {
                    curiousTile.Reveal();
                    RevealEmptyNeighbours(Vei2(xi, yi));
                    
                }
                else {
                    curiousTile.Reveal();
                    return;
                }
            }
            

        }
    }

}


void MemeField::FlagOnCLick(const Vei2& screenPos)
{
    Vei2 gridPos = (screenPos - topleft)/ SpriteCodex::tileSize;
    assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
    Tile& interestedTile = TileAt(gridPos);

    if (!interestedTile.IsFlagged()) {
        interestedTile.Flag(true);
    }
    else {
        interestedTile.Flag(false);
    }
}



MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) 
{
    return Field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
    return Field[gridPos.y * width + gridPos.x];
}


void MemeField::SetGameOver()
{
    memeStepped = true;

    // Reveal all tiles with memes
    for (Vei2 gridPos = { 0, 0}; gridPos.y < height; gridPos.y++) {
        for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {

            if (TileAt(gridPos).HasMeme()) {
                TileAt(gridPos).Reveal();
            }
        }
    }

    
}
