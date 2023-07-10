#pragma once
#include <vector>
#include "TextureManager.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Tile{
public:
    sf::Sprite backgroundTile;
    sf::Sprite coverTile;
    sf::Sprite flag;
    sf::Sprite mine;
    sf::Sprite num;

    bool flagged;
    bool revealed;
    bool hasMine;
    bool debug;
    vector<Tile*> adjacentTiles;
    int adjacentMineTiles;
    Tile(float x, float y);
    //void setHasMine(bool _hasMine);
    void draw(sf::RenderWindow& window) const;
    void debugMode();
    void findAdjacent();
};

Tile::Tile(float x, float y) {
flagged = false;
revealed = false;
hasMine = false;
debug = false;
adjacentMineTiles = 0;
adjacentTiles.resize(8, nullptr);
backgroundTile.setTexture(TextureManager::GetTexture("tile_revealed"));
backgroundTile.setPosition(x, y);
coverTile.setTexture(TextureManager::GetTexture("tile_hidden"));
coverTile.setPosition(x, y);
flag.setTexture(TextureManager::GetTexture("flag"));
flag.setPosition(x, y);
num.setPosition(x, y);
mine.setPosition(x, y);
mine.setTexture(TextureManager::GetTexture("mine"));
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(coverTile);
    if (hasMine && revealed) {
        window.draw(backgroundTile);
        window.draw(mine);
    }
    if (revealed && !hasMine) {
        window.draw(backgroundTile);
        window.draw(num);
    }
    if (flagged) {
        window.draw(flag);
    }
    if(debug && hasMine){
        window.draw(coverTile);
        window.draw(mine);
        if(flagged){
            window.draw(flag);
        }
    }
}

/*    void Tile::setHasMine(bool _hasMine) {
        hasMine = _hasMine;
        if(hasMine) {
            mine.setTexture(TextureManager::GetTexture("mine"));
        }
    }
*/
    void Tile::debugMode() {
        debug = !debug;
        if (debug) {
            revealed = true;
        }
        else {
            revealed = false;
        }
    }

    void Tile::findAdjacent() {

        for (int i = 0; i < adjacentTiles.size(); ++i) {
            if (adjacentTiles[i] != nullptr && adjacentTiles[i]->hasMine) {
                adjacentMineTiles++;
            }
        }
        if (adjacentMineTiles == 1) {
            num.setTexture(TextureManager::GetTexture("number_1"));
        } else if (adjacentMineTiles == 2) {
            num.setTexture(TextureManager::GetTexture("number_2"));
        } else if (adjacentMineTiles == 3) {
            num.setTexture(TextureManager::GetTexture("number_3"));
        } else if (adjacentMineTiles == 4) {
            num.setTexture(TextureManager::GetTexture("number_4"));
        } else if (adjacentMineTiles == 5) {
            num.setTexture(TextureManager::GetTexture("number_5"));
        } else if (adjacentMineTiles == 6) {
            num.setTexture(TextureManager::GetTexture("number_6"));
        } else if (adjacentMineTiles == 7) {
            num.setTexture(TextureManager::GetTexture("number_7"));
        } else if (adjacentMineTiles == 8) {
            num.setTexture(TextureManager::GetTexture("number_8"));
        }
    }
