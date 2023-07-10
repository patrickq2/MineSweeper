#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "TextureManager.h"
#include "Random.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Board {

    vector<vector<Tile>> tiles;
    int columns;
    int rows;
    int mines;
    int flags;
    bool gameOver;
    sf::Sprite face;
    sf::Sprite debug;
    sf::Sprite test1;
    sf::Sprite test2;
    sf::Sprite test3;
    sf::Sprite digit1;
    sf::Sprite digit2;
    sf::Sprite digit3;
    sf::Sprite negative;

public:
    Board();
    void draw(sf::RenderWindow& window);
    void play(sf::RenderWindow& window,  sf::Event &event);
    void reset();
    void setBombs();
    void findAdjacentBombs();
    int getWidth() const;
    int getHeight() const;
    void updateCounter();
    void tests(string boardName);
    void endgame(bool won);
    void checkWinner();
    void recursiveReveal(Tile* tile);
};

Board::Board() {
    string path = "boards/config.cfg";
    fstream file(path);
    string line;
    getline(file, line);
    columns = stoi(line);
    getline(file, line);
    rows = stoi(line);
    getline(file, line);
    mines = stoi(line);
    flags = mines;
    gameOver = false;
    for (int i = 0; i < rows; i++) {
        tiles.push_back(vector<Tile>());
        for (int j = 0; j < columns; j++) {
            Tile temp(j * 32.0f, i * 32.0f);
            tiles.at(i).push_back(temp);
        }
    }
    face.setPosition(getWidth()/2.0f - 32.0f,getHeight() - 100.0f);
    face.setTexture(TextureManager::GetTexture("face_happy"));
    debug.setPosition(getWidth()/2.0f + 96.0f, getHeight() - 100.0f);
    debug.setTexture(TextureManager::GetTexture("debug"));
    test1.setPosition(getWidth()/2.0f + 160.0f, getHeight() - 100.0f);
    test1.setTexture(TextureManager::GetTexture("test_1"));
    test2.setPosition(getWidth()/2.0f + 224.0f, getHeight() - 100.0f);
    test2.setTexture(TextureManager::GetTexture("test_2"));
    test3.setPosition(getWidth()/2.0f + 288.0f, getHeight() - 100.0f);
    test3.setTexture(TextureManager::GetTexture("test_3"));
    negative.setPosition(0, getHeight()-100);
    negative.setTexture(TextureManager::GetTexture("digits"));
    negative.setTextureRect(sf::IntRect((10*21), 0, 21, 32));
    digit1.setPosition(21 ,getHeight()-100);
    digit1.setTexture(TextureManager::GetTexture("digits"));
    digit1.setTextureRect(sf::IntRect((flags/100)*21, 0, 21, 32));
    digit2.setPosition(42 ,getHeight()-100);
    digit2.setTexture(TextureManager::GetTexture("digits"));
    digit2.setTextureRect(sf::IntRect((flags%100/10)*21, 0, 21, 32));
    digit3.setPosition(63 ,getHeight()-100);
    digit3.setTexture(TextureManager::GetTexture("digits"));
    digit3.setTextureRect(sf::IntRect(((flags%100)%10)*21, 0, 21, 32));
    setBombs();
    findAdjacentBombs();
}

void Board::draw(sf::RenderWindow &window) {
    for (int i = 0; i < tiles.size(); i++) {
       for (int j = 0; j < tiles[i].size(); j++) {
            tiles[i][j].draw(window);
       }
    }

    window.draw(face);
    window.draw(debug);
    window.draw(test1);
    window.draw(test2);
    window.draw(test3);
    if(flags < 0){
    window.draw(negative);
    }
    window.draw(digit1);
    window.draw(digit2);
    window.draw(digit3);
}

int Board::getWidth() const {
    return columns * 32;
}

int Board::getHeight() const {
    return (rows * 32) + 100;
}

void Board::setBombs() {
    vector<int> mineLoc;
    bool duplicate;
    for (int i = 0; i < mines; i++) {
        duplicate = true;
        while(duplicate) {
            int location = Random::Number(0,(rows * columns) - 1);
            if(find(mineLoc.begin(), mineLoc.end(), location) == mineLoc.end()){
                mineLoc.push_back(location);
                int tileCounter = 0;
                for(int i = 0; i < tiles.size(); i++) {
                    for (int j = 0; j < tiles.at(i).size(); j++) {
                        if(tileCounter == location){
                            tiles[i][j].hasMine = true;
                            duplicate = false;
                            break;
                        }
                        else {
                            tileCounter++;
                        }
                    }
                    if(!duplicate) {
                        break;
                    }
                }
            }

        }
    }
    mineLoc.clear();
}

void Board::findAdjacentBombs() {
    /*
        0       1       2

        3      Tile     4

        5       6       7
    */

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if (!tiles[i][j].hasMine) {
                if(i == 0){
                    if(j != 0 && j != tiles[i].size()-1){
                        tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                        tiles[i][j].adjacentTiles[4] = &tiles[i][j+1];
                        tiles[i][j].adjacentTiles[5] = &tiles[i+1][j-1];
                        tiles[i][j].adjacentTiles[7] = &tiles[i+1][j+1];
                    }
                    else if(j == 0){
                        tiles[i][j].adjacentTiles[4] = &tiles[i][j+1];
                        tiles[i][j].adjacentTiles[7] = &tiles[i+1][j+1];
                    }
                    else if(j == tiles[i].size()-1){
                        tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                        tiles[i][j].adjacentTiles[5] = &tiles[i][j+1];
                    }
                    tiles[i][j].adjacentTiles[6] = &tiles[i+1][j];
                }

                else if(i == tiles.size()-1){
                    if(j != 0 && j != tiles[i].size()-1){
                        tiles[i][j].adjacentTiles[0] = &tiles[i-1][j-1];
                        tiles[i][j].adjacentTiles[2] = &tiles[i-1][j+1];
                        tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                        tiles[i][j].adjacentTiles[4] = &
                                tiles[i][j+1];
                    }
                    else if(j == 0){
                        tiles[i][j].adjacentTiles[2] = &tiles[i-1][j+1];
                        tiles[i][j].adjacentTiles[4] = &tiles[i][j+1];
                    }
                    else if(j == tiles[i].size()-1){
                        tiles[i][j].adjacentTiles[0] = &tiles[i-1][j-1];
                        tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                    }
                    tiles[i][j].adjacentTiles[1] = &tiles[i-1][j];
                }

                else if(j==0){
                   tiles[i][j].adjacentTiles[1] = &tiles[i-1][j];
                   tiles[i][j].adjacentTiles[2] = &tiles[i-1][j+1];
                   tiles[i][j].adjacentTiles[4] = &tiles[i][j+1];
                   tiles[i][j].adjacentTiles[6] = &tiles[i+1][j];
                   tiles[i][j].adjacentTiles[7] = &tiles[i+1][j+1];
                }

                else if(j == tiles[i].size()-1){
                    tiles[i][j].adjacentTiles[0] = &tiles[i-1][j-1];
                    tiles[i][j].adjacentTiles[1] = &tiles[i-1][j];
                    tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                    tiles[i][j].adjacentTiles[5] = &tiles[i+1][j-1];
                    tiles[i][j].adjacentTiles[6] = &tiles[i+1][j];
                }

                else{
                   tiles[i][j].adjacentTiles[0] = &tiles[i-1][j-1];
                   tiles[i][j].adjacentTiles[1] = &tiles[i-1][j];
                   tiles[i][j].adjacentTiles[2] = &tiles[i-1][j+1];
                   tiles[i][j].adjacentTiles[3] = &tiles[i][j-1];
                   tiles[i][j].adjacentTiles[4] = &tiles[i][j+1];
                   tiles[i][j].adjacentTiles[5] = &tiles[i+1][j-1];
                   tiles[i][j].adjacentTiles[6] = &tiles[i+1][j];
                   tiles[i][j].adjacentTiles[7] = &tiles[i+1][j+1];
                }


                tiles[i][j].findAdjacent();
            }
        }
    }
}

void Board::play(sf::RenderWindow &window, sf::Event &event) {
    draw(window);

    if(event.type == sf::Event::MouseButtonPressed){
        sf::Vector2f mousePos =  window.mapPixelToCoords(sf::Mouse::getPosition(window)); //found on sfml web forum

        if(event.mouseButton.button == sf::Mouse::Left) {
            if(!gameOver) {
                for (int i = 0; i < tiles.size(); i++) {
                    for (int j = 0; j < tiles[i].size(); j++) {
                        if (!tiles[i][j].flagged) {
                            if (tiles[i][j].backgroundTile.getGlobalBounds().contains(mousePos)) {
                                if (tiles[i][j].hasMine) {
                                    endgame(false);
                                }
                                else{
                                    recursiveReveal(&tiles[i][j]);
                                }
                            }
                        }
                    }
                }
            }

            if (face.getGlobalBounds().contains(mousePos)) {
                reset();
            }
            if (debug.getGlobalBounds().contains(mousePos) && !gameOver) {
                for (int i = 0; i < tiles.size(); i++) {
                    for (int j = 0; j < tiles[i].size(); j++) {
                        if (tiles[i][j].hasMine) {
                            tiles[i][j].debugMode();
                        }
                    }
                }
            }
            if(test1.getGlobalBounds().contains(mousePos)){
               tests("testboard1");
            }
            if(test2.getGlobalBounds().contains(mousePos)){
                tests("testboard2");
            }
            if(test3.getGlobalBounds().contains(mousePos)){
                tests("testboard3");
            }
        }
        if(event.mouseButton.button == sf::Mouse::Right && !gameOver){
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles[i].size(); j++) {
                    if(tiles[i][j].backgroundTile.getGlobalBounds().contains(mousePos)){
                        if(!tiles[i][j].revealed || tiles[i][j].debug) {
                            tiles[i][j].flagged = !tiles[i][j].flagged;
                            if (tiles[i][j].flagged) {
                                flags--;
                                updateCounter();
                            }
                            else {
                                flags++;
                                updateCounter();
                            }
                        }
                    }
                }
            }
        }
        event.type = sf::Event::MouseButtonReleased;
    }
    checkWinner();
}

void Board::updateCounter() {

    digit1.setTextureRect(sf::IntRect((abs(flags)/100)*21, 0, 21, 32));
    digit2.setTextureRect(sf::IntRect((abs(flags)%100/10)*21, 0, 21, 32));
    digit3.setTextureRect(sf::IntRect((abs(flags)%100%10)*21, 0, 21, 32));

}

void Board::reset() {
    tiles.clear();
    flags = mines;
    updateCounter();
    for (int i = 0; i < rows; i++) {
        tiles.push_back(vector<Tile>());
        for (int j = 0; j < columns; j++) {
            Tile temp(j * 32.0f, i * 32.0f);
            tiles[i].push_back(temp);
        }
    }
    setBombs();
    findAdjacentBombs();
    face.setTexture(TextureManager::GetTexture("face_happy"));
    gameOver = false;
}

void Board::endgame(bool won) {

    gameOver = true;
    if(won){
        face.setTexture(TextureManager::GetTexture("face_Win"));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if(!tiles[i][j].flagged){
                    tiles[i][j].revealed = true;
                }
            }
        }
    }
    else{
        face.setTexture(TextureManager::GetTexture("face_lose"));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if(tiles[i][j].hasMine){
                    tiles[i][j].revealed = true;
                    tiles[i][j].flagged = false;
                }
            }
        }
    }

}

void Board::checkWinner() {
    int flagCounter = 0;
    int revealedTiles = 0;
    if(flags == 0){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if(tiles[i][j].hasMine && tiles[i][j].flagged){
                    flagCounter++;
                }
            }
        }
        if(flagCounter == mines){
            endgame(true);
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if(tiles[i][j].revealed){
                revealedTiles++;
            }
        }
    }
    if(revealedTiles == (rows*columns)-mines){
        endgame(true);
    }
}

void Board::recursiveReveal(Tile* tile) {
    tile->revealed = true;
    if (tile->adjacentMineTiles == 0) {
        for (int i = 0; i < tile->adjacentTiles.size(); ++i) {
            if (tile->adjacentTiles[i] != nullptr && !tile->adjacentTiles[i]->flagged && !tile->adjacentTiles[i]->revealed) {
                recursiveReveal(tile->adjacentTiles[i]);
            }
        }
    }
}

void Board::tests(string boardName) {
    tiles.clear();
    mines = 0;
    vector<int>mineLoc;
    string fileName = "boards/" + boardName + ".brd";
    string line;
    int counter = 0;
    fstream file(fileName);
    while(getline(file, line)){
        string::iterator iter;
        for (iter = line.begin(); iter != line.end(); iter++) {
            mineLoc.push_back((*iter) - 48);
            if(((*iter) - 48)  == 1){
                mines++;
            }
        }
    }
    for (int i = 0; i < rows; i++) {
        tiles.push_back(vector<Tile>());
        for (int j = 0; j < columns; j++) {
            Tile temp(j * 32.0f, i * 32.0f);
            tiles[i].push_back(temp);
        }
    }
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(mineLoc[counter] == 1){
                tiles[i][j].hasMine = true;
            }
            counter++;
        }
    }
    flags = mines;
    updateCounter();
    findAdjacentBombs();
    face.setTexture(TextureManager::GetTexture("face_happy"));
    gameOver = false;
}




