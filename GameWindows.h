#pragma once
#include "File_Input.h"
#include "Timer.h"
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace sf;
using namespace std;

class GameWindows {
    private:
        FileReader writeFile;
        Sprite mineSprite, hiddenTileSprite, revealedTileSprite,
                happyFaceButtonSprite, debugButtonSprite, pauseButtonSprite,
                leaderboardButtonSprite, counterSprite, playButtonSprite,
                numberOneSprite, numberTwoSprite, numberThreeSprite,
                numberFourSprite, numberFiveSprite, numberSixSprite,
                numberSevenSprite, numberEightSprite, flagSprite, winFaceSprite, loseFaceSprite;

        Texture mineTexture, hiddenTileTexture, revealedTileTexture,
                happyFaceButtonTexture, debugButtonTexture, pauseButtonTexture,
                leaderboardButtonTexture, counterTexture, playButtonTexture,
                numberOneTexture, numberTwoTexture, numberThreeTexture,
                numberFourTexture, numberFiveTexture, numberSixTexture,
                numberSevenTexture, numberEightTexture, flagTexture, winFaceTexture,
                loseFaceTexture;

        FloatRect revealedTileSpriteBounds;
        FloatRect debugButtonBounds;
        FloatRect leaderboardButtonBounds;
        FloatRect pauseAndPlayBounds;
        FloatRect happyButtonBounds;

        bool isLeaderboardDisplayed = false;
        bool wasPauseClicked = false;
        bool wasResetClicked = false;
        bool wasResumeClicked = false;
        bool abilityToRightClick = true;
        bool wasLeaderClicked = false;

        bool allowedToClickHappy = true;
        bool allowedToClickDebug = true;
        bool allowedToClickLeaderboard = true;
        bool allowedToClickPausePlay = true;
        bool allowedToClickTiles = true;

        string playerName;

        Font font;
        map<string, string> leaderboard;
        Timer timer;
        int minesRIGHTNOW;
        int flagsAvailable;
        int minesAvailable;
        int minePlacement;
        struct Tile {
            Tile* TopLeft = nullptr;
            Tile* TopRight = nullptr;
            Tile* Top = nullptr;
            Tile* Left = nullptr;
            Tile* Right = nullptr;
            Tile* BottomRight = nullptr;
            Tile* Bottom = nullptr;
            Tile* BottomLeft = nullptr;

            bool isBomb = false;
            bool isRevealed = false;
            bool isFlagged = false;

            int neighboringBombs = 0;

            Tile(bool input) {
                isBomb = input;
            }

            ~Tile() {

            }
        };
        vector<vector<Tile*>> Tiles;
        vector<int> mineLocations;

    public:
        void leaderboardSetup(map<string, string> ldrbrd, Font font){
            leaderboard = ldrbrd;
            this->font = font;
        }
        void clearBoard() {
            for (auto& row : Tiles) {
                for (auto& tile : row) {
                    delete tile; // Free memory allocated for each Tile
                }
                row.clear();
            }
            Tiles.clear();
            mineLocations.clear();
        }
        void randomizeBoard(int rows, int columns, int mines) {
            clearBoard();
            minesAvailable = mines;

            // Randomly place mines
            while (minesAvailable != 0) {
                minePlacement = rand() % (rows * columns);
                if (find(mineLocations.begin(), mineLocations.end(), minePlacement) == mineLocations.end()) {
                    mineLocations.push_back(minePlacement);
                    minesAvailable--;
                }
            }

            // Initialize the tiles
            Tiles.resize(rows, vector<Tile*>(columns, nullptr));
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    bool isItMine = find(mineLocations.begin(), mineLocations.end(), i * columns + j) != mineLocations.end();
                    Tiles[i][j] = new Tile(isItMine);
                }
            }

            // Link Tiles together
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    if (i > 0) {
                        Tiles[i][j]->Top = Tiles[i - 1][j];

                        if (j > 0){
                            Tiles[i][j]->TopLeft = Tiles[i - 1][j - 1];
                        }

                        if (j < columns - 1){
                            Tiles[i][j]->TopRight = Tiles[i - 1][j + 1];
                        }
                    }
                    if (i < rows - 1) {
                        Tiles[i][j]->Bottom = Tiles[i + 1][j];

                        if (j > 0){
                            Tiles[i][j]->BottomLeft = Tiles[i + 1][j - 1];
                        }

                        if (j < columns - 1){
                            Tiles[i][j]->BottomRight = Tiles[i + 1][j + 1];
                        }
                    }
                    if (j > 0){
                        Tiles[i][j]->Left = Tiles[i][j - 1];
                    }
                    if (j < columns - 1) {
                        Tiles[i][j]->Right = Tiles[i][j + 1];
                    }
                }
            }

            // Neighboring Bombs Counting
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    int count = 0;
                    if (!(Tiles[i][j]->isBomb)) {

                        if (Tiles[i][j]->Top != nullptr && Tiles[i][j]->Top->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->TopLeft != nullptr && Tiles[i][j]->TopLeft->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->TopRight != nullptr && Tiles[i][j]->TopRight->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->Right != nullptr && Tiles[i][j]->Right->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->Left != nullptr && Tiles[i][j]->Left->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->BottomLeft != nullptr && Tiles[i][j]->BottomLeft->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->Bottom != nullptr && Tiles[i][j]->Bottom->isBomb) {
                            count++;
                        }

                        if (Tiles[i][j]->BottomRight != nullptr && Tiles[i][j]->BottomRight->isBomb) {
                            count++;
                        }

                        Tiles[i][j]->neighboringBombs = count;
                    }
                }
            }
        }
        bool didPlayerWin() {
            int count = 0;
            for (int i = 0; i < Tiles.size(); i++) {
                for (int j = 0; j < Tiles[i].size(); j++){
                    if (!(Tiles[i][j]->isBomb) && Tiles[i][j]->isRevealed) {
                        count++;
                    }
                }
            }
            int multiply = (Tiles.size()*Tiles[0].size());
            //cout <<"Count: " << count<< " Mines: " << minesRIGHTNOW <<endl;
            //cout << "Board Size: " << multiply <<endl;
            return (multiply == (count + minesRIGHTNOW));
        }
        vector<int> minCounterDigits(int num){
            vector<int> digits;
            int mins = num/60;
            digits.push_back(mins/10);
            digits.push_back(mins%10);
            return digits;
        }
        vector<int> secondCounterDigits(int num){
            vector<int> digits;
            int seconds = num%60;
            digits.push_back(seconds/10);
            digits.push_back(seconds%10);

            //reverse(digits.begin(), digits.end());
            return digits;
        }
        vector<int> counterDigits(int num){

            vector<int> digits;
            int temp = abs(num);
            while(temp != 0){
                digits.push_back(temp % 10); // Use temp to get the digits
                temp = temp / 10;
            }
            if(digits.size() == 0){
                digits.push_back(0);
                digits.push_back(0);
                digits.push_back(0);
            }
            if(digits.size() == 1){
                digits.push_back(0);
                digits.push_back(0);
            }
            if(digits.size() == 2){
                digits.push_back(0);
            }

            reverse(digits.begin(), digits.end()); // Digits need to be reversed
            return digits;
        }
        void setText(Text& text, const string& str, const Font& font, unsigned int size, float x, float y, Color color) {
            text.setFont(font);
            text.setString(str);
            text.setCharacterSize(size);
            text.setFillColor(color);
            FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            text.setPosition(x, y);
        }
        void displayLeaderboard(int columns, int rows) {
            RenderWindow leaderboardWin(VideoMode((columns * 16), ((rows * 16) + 50)), "Minesweeper", Style::Close);
            timer.pause();
            leaderboardWin.clear(Color::Blue);

            Text title;
            setText(title, "LEADERBOARD", font, 20, leaderboardWin.getSize().x / 2.0f, (leaderboardWin.getSize().y / 2.0f) - 120, Color::White);
            title.setStyle(Text::Bold | Text::Underlined);
            leaderboardWin.draw(title);

            float yPosition = leaderboardWin.getSize().y / 2.0f - 70;
            int counter = 1;
            float rankX = leaderboardWin.getSize().x / 2.0f - 110;
            float timeX = leaderboardWin.getSize().x / 2.0f - 10;
            float nameX = leaderboardWin.getSize().x / 2.0f + 100 - 10;

            for (const auto& entry : leaderboard) {
                if(entry.first != "") {
                    Text rankText;
                    setText(rankText, to_string(counter) + '.', font, 18, rankX, yPosition, Color::White);
                    rankText.setStyle(Text::Bold);
                    leaderboardWin.draw(rankText);

                    Text timeText;
                    setText(timeText, entry.first, font, 18, timeX, yPosition, Color::White);
                    timeText.setStyle(Text::Bold);
                    leaderboardWin.draw(timeText);

                    Text nameText;
                    setText(nameText, entry.second, font, 18, nameX, yPosition, Color::White);
                    nameText.setStyle(Text::Bold);
                    leaderboardWin.draw(nameText);

                    yPosition += 40;
                    counter++;
                }
            }

            leaderboardWin.display();
            Event event;
            while (leaderboardWin.waitEvent(event)) {
                isLeaderboardDisplayed = true;
                if(didPlayerWin()){
                    allowedToClickHappy = true;
                }
                if (event.type == Event::Closed) {
                    isLeaderboardDisplayed = false;
                    leaderboardWin.close();

                    allowedToClickTiles = true;
                    allowedToClickDebug = true;
                    allowedToClickLeaderboard = true;
                    allowedToClickPausePlay = true;
                    allowedToClickHappy = true;

                    if(!wasPauseClicked){
                        timer.resume();
                    }
                    break;
                }
            }
        }
        void updateLeaderboard(int num){
            int min = num/60;
            int sec = num%60;

            int digitOne = min/10;
            int digitTwo = min%10;
            int digitThree = sec/10;
            int digitFour = sec%10;

            string playerTime = to_string(digitOne) + to_string(digitTwo)
                    + ':' + to_string(digitThree) + to_string(digitFour);

            leaderboard.emplace(playerTime, playerName);
            writeFile.setLeaderboard(leaderboard);
            writeFile.writeLeaderboard();

        }
        bool clearTiles(Tile* tile){
        if (tile == nullptr || tile->isRevealed || tile->isBomb || tile->isFlagged) {
            return true; // Base case: if tile is null or already revealed, stop recursion
        }
        else if(tile->neighboringBombs > 0) {
            tile->isRevealed = true;
             // Mark tile as revealed
            return true; // Stop recursion if tile has neighboring bombs
        }
        else{
            tile->isRevealed = true;
        }

        // Recursively clear adjacent tiles
        clearTiles(tile->Top);
        clearTiles(tile->TopLeft);
        clearTiles(tile->TopRight);
        clearTiles(tile->Right);
        clearTiles(tile->Left);
        clearTiles(tile->BottomLeft);
        clearTiles(tile->Bottom);
        clearTiles(tile->BottomRight);

        return true;
    }
        void gameWindow(RenderWindow& window, int rows, int columns, int mines, string Name) {
            minesRIGHTNOW = mines;
            minesAvailable = mines;
            flagsAvailable = mines;

            for(int i = 0; i < Name.length(); i++){
                if(i == 0){
                    playerName += toupper(Name[i]);
                }
                else{
                    playerName += tolower(Name[i]);
                }
            }
            playerName += '*';

            mineTexture.loadFromFile("files/images/mine.png");
            hiddenTileTexture.loadFromFile("files/images/tile_hidden.png");
            revealedTileTexture.loadFromFile("files/images/tile_revealed.png");
            happyFaceButtonTexture.loadFromFile("files/images/face_happy.png");
            debugButtonTexture.loadFromFile("files/images/debug.png");
            pauseButtonTexture.loadFromFile("files/images/pause.png");
            playButtonTexture.loadFromFile("files/images/play.png");
            leaderboardButtonTexture.loadFromFile("files/images/leaderboard.png");
            counterTexture.loadFromFile("files/images/digits.png");
            flagTexture.loadFromFile("files/images/flag.png");
            winFaceTexture.loadFromFile("files/images/face_win.png");
            loseFaceTexture.loadFromFile("files/images/face_lose.png");

            numberOneTexture.loadFromFile("files/images/number_1.png");
            numberTwoTexture.loadFromFile("files/images/number_2.png");
            numberThreeTexture.loadFromFile("files/images/number_3.png");
            numberFourTexture.loadFromFile("files/images/number_4.png");
            numberFiveTexture.loadFromFile("files/images/number_5.png");
            numberSixTexture.loadFromFile("files/images/number_6.png");
            numberSevenTexture.loadFromFile("files/images/number_7.png");
            numberEightTexture.loadFromFile("files/images/number_8.png");

            //Sprites
            mineSprite.setTexture(mineTexture);
            hiddenTileSprite.setTexture(hiddenTileTexture);
            revealedTileSprite.setTexture(revealedTileTexture);
            happyFaceButtonSprite.setTexture(happyFaceButtonTexture);
            debugButtonSprite.setTexture(debugButtonTexture);
            pauseButtonSprite.setTexture(pauseButtonTexture);
            leaderboardButtonSprite.setTexture(leaderboardButtonTexture);
            counterSprite.setTexture(counterTexture);
            playButtonSprite.setTexture(playButtonTexture);
            flagSprite.setTexture(flagTexture);
            numberOneSprite.setTexture(numberOneTexture);
            numberTwoSprite.setTexture(numberTwoTexture);
            numberThreeSprite.setTexture(numberThreeTexture);
            numberFourSprite.setTexture(numberFourTexture);
            numberFiveSprite.setTexture(numberFiveTexture);
            numberSixSprite.setTexture(numberSixTexture);
            numberSevenSprite.setTexture(numberSevenTexture);
            numberEightSprite.setTexture(numberEightTexture);
            winFaceSprite.setTexture(winFaceTexture);
            loseFaceSprite.setTexture(loseFaceTexture);

            //Bounds
            revealedTileSpriteBounds = revealedTileSprite.getGlobalBounds();
            debugButtonBounds = debugButtonSprite.getGlobalBounds();
            leaderboardButtonBounds = leaderboardButtonSprite.getGlobalBounds();
            pauseAndPlayBounds = pauseButtonSprite.getGlobalBounds();
            happyButtonBounds = happyFaceButtonSprite.getGlobalBounds();

            //Set Positions for imagePositions
            happyFaceButtonSprite.setPosition(((columns / 2 * 32) - 32), (32 * (rows + 0.5)));
            winFaceSprite.setPosition(((columns / 2 * 32) - 32), (32 * (rows + 0.5)));
            loseFaceSprite.setPosition(((columns / 2 * 32) - 32), (32 * (rows + 0.5)));
            debugButtonSprite.setPosition(((columns * 32) - 304), (32 * (rows + 0.5)));
            pauseButtonSprite.setPosition(((columns * 32) - 240), (32 * (rows + 0.5)));
            playButtonSprite.setPosition(((columns * 32) - 240), (32 * (rows + 0.5)));
            leaderboardButtonSprite.setPosition(((columns * 32) - 176), (32 * (rows + 0.5)));

            randomizeBoard(rows, columns, mines);

            Clock clock;
            timer.start();

            bool lost = false;
            bool displayPause = true;
            bool debugMode = false;
            bool needToDisplayLdrBoard = false;
            bool didIShowNewLeaderboard = false;

            while (window.isOpen()) {
                //cout<<"Como te llamas? Me llamo " << playerName <<endl;
                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::MouseButtonPressed) {
                        Vector2i mousePos = Mouse::getPosition(window);
                        int x = mousePos.x / revealedTileSpriteBounds.width;
                        int y = mousePos.y / revealedTileSpriteBounds.height;

                        if (event.mouseButton.button == Mouse::Left) {
                            if (y >= 0 && y < rows && x >= 0 && x < columns) {
                                if (!Tiles[y][x]->isRevealed && allowedToClickTiles) {
                                    if (Tiles[y][x]->isBomb) {
                                        // Handle game over condition
                                        lost = true;
                                        allowedToClickTiles = false;
                                        allowedToClickPausePlay = false;
                                        allowedToClickDebug = false;
                                        abilityToRightClick = false;

                                        timer.stop();
                                    } else{
                                        // Optionally, reveal adjacent tiles recursively if no neighboring bombs
                                        clearTiles(Tiles[y][x]);
                                    }
                                }
                            } else if (mousePos.x >= happyFaceButtonSprite.getPosition().x &&
                                       mousePos.x < (happyFaceButtonSprite.getPosition().x + happyButtonBounds.width) &&
                                       mousePos.y >= happyFaceButtonSprite.getPosition().y &&
                                       mousePos.y < (happyFaceButtonSprite.getPosition().y + happyButtonBounds.height) &&
                                       allowedToClickHappy) {

                                //cout << "CLAP ALONG IF YOU FEEL LIKE YOURE IN A ROOM WITHOUT A ROOF CAUSE IM HAPPY - Pharell Williams" << endl;
                                randomizeBoard(rows, columns, mines);
                                debugMode = false;
                                didIShowNewLeaderboard = false;
                                flagsAvailable = mines;
                                wasResetClicked = true;
                                if(!wasPauseClicked && !wasResumeClicked){
                                    timer.start();
                                }

                                if(lost){
                                    lost = false;
                                    allowedToClickTiles = true;
                                    allowedToClickPausePlay = true;
                                    allowedToClickDebug = true;
                                    abilityToRightClick = true;
                                    window.draw(happyFaceButtonSprite);
                                    timer.start();
                                }
                            } else if (mousePos.x >= pauseButtonSprite.getPosition().x &&
                                       mousePos.x < (pauseButtonSprite.getPosition().x + pauseAndPlayBounds.width) &&
                                       mousePos.y >= pauseButtonSprite.getPosition().y &&
                                       mousePos.y < (pauseButtonSprite.getPosition().y + pauseAndPlayBounds.height) &&
                                       allowedToClickPausePlay && !lost) {
                                //cout << "HOLD ON WAIT A MINUTE FILL MY CUP PUT SOME LIQUOR IN IT - Bruno Mars" << endl;
                                if (displayPause) {
                                    displayPause = false;
                                    allowedToClickTiles = false;
                                    allowedToClickDebug = false;
                                    wasPauseClicked = true;
                                    timer.pause();
                                } else {
                                    wasResumeClicked = true;
                                    allowedToClickTiles = true;
                                    allowedToClickDebug = true;
                                    displayPause = true;
                                    timer.resume();
                                }
                            } else if (mousePos.x >= debugButtonSprite.getPosition().x &&
                                       mousePos.x < (debugButtonSprite.getPosition().x + debugButtonBounds.width) &&
                                       mousePos.y >= debugButtonSprite.getPosition().y &&
                                       mousePos.y < (debugButtonSprite.getPosition().y + debugButtonBounds.height) &&
                                       allowedToClickDebug) {
                                //cout << "crodie got me buggin" << endl;

                                if(!debugMode){
                                    debugMode = true;
                                }
                                else{
                                    debugMode = false;
                                }

                            } else if (mousePos.x >= leaderboardButtonSprite.getPosition().x &&
                                       mousePos.x < (leaderboardButtonSprite.getPosition().x + leaderboardButtonBounds.width) &&
                                       mousePos.y >= leaderboardButtonSprite.getPosition().y &&
                                       mousePos.y < (leaderboardButtonSprite.getPosition().y + leaderboardButtonBounds.height) &&
                                       allowedToClickLeaderboard) {
                                //cout<<"I clicked in leader"<<endl;
                                if (!needToDisplayLdrBoard && !isLeaderboardDisplayed) {
                                    needToDisplayLdrBoard = true;
                                    wasLeaderClicked = true;
                                    allowedToClickTiles = false;
                                    allowedToClickDebug = false;
                                    allowedToClickPausePlay = false;
                                    allowedToClickHappy = false;
                                    //cout<<"leader is about to show"<<endl;
                                }
                            }
                        } else if (event.mouseButton.button == sf::Mouse::Right) {
                            if (y >= 0 && y < rows && x >= 0 && x < columns) {
                                if(abilityToRightClick) {
                                    if (!Tiles[y][x]->isRevealed && allowedToClickTiles) {
                                        if (!Tiles[y][x]->isFlagged && !Tiles[y][x]->isRevealed) {
                                            Tiles[y][x]->isFlagged = true;
                                            flagsAvailable--;
                                        } else {
                                            Tiles[y][x]->isFlagged = false;
                                            flagsAvailable++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (clock.getElapsedTime().asSeconds() >= 0.01) {
                    clock.restart();
                    window.clear(Color::White);

                    for (int i = 0; i < Tiles.size(); i++) {
                        for (int j = 0; j < Tiles[i].size(); j++) {
                            float xShift = j * revealedTileSpriteBounds.width;
                            float yShift = i * revealedTileSpriteBounds.height;

                            revealedTileSprite.setPosition(xShift, yShift);
                            window.draw(revealedTileSprite);

                            if (Tiles[i][j]->isBomb) {

                                mineSprite.setPosition((xShift), yShift);
                                window.draw(mineSprite);

                            } else {
                                if (Tiles[i][j]->neighboringBombs == 1) {
                                    numberOneSprite.setPosition(xShift, yShift);
                                    window.draw(numberOneSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 2) {
                                    numberTwoSprite.setPosition(xShift, yShift);
                                    window.draw(numberTwoSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 3) {
                                    numberThreeSprite.setPosition(xShift, yShift);
                                    window.draw(numberThreeSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 4) {
                                    numberFourSprite.setPosition(xShift, yShift);
                                    window.draw(numberFourSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 5) {
                                    numberFiveSprite.setPosition(xShift, yShift);
                                    window.draw(numberFiveSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 6) {
                                    numberSixSprite.setPosition(xShift, yShift);
                                    window.draw(numberSixSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 7) {
                                    numberSevenSprite.setPosition(xShift, yShift);
                                    window.draw(numberSevenSprite);
                                } else if (Tiles[i][j]->neighboringBombs == 8) {
                                    numberEightSprite.setPosition(xShift, yShift);
                                    window.draw(numberEightSprite);
                                }
                            }
                            if (!Tiles[i][j]->isRevealed) {
                                hiddenTileSprite.setPosition(xShift, yShift);
                                window.draw(hiddenTileSprite);
                            }

                            if (Tiles[i][j]->isFlagged) {
                                flagSprite.setPosition(xShift, yShift);
                                window.draw(flagSprite);
                            }

                            //cout <<"IM STILL PRINTING"<<endl;
                        }
                    }

                    if(didPlayerWin()){
                        for(int i = 0; i < rows; i++) {
                            for (int j = 0; j < columns; j++) {
                                float xShift = j * revealedTileSpriteBounds.width;
                                float yShift = i * revealedTileSpriteBounds.height;
                                if(Tiles[i][j]->isBomb) {
                                    flagSprite.setPosition((xShift), yShift);
                                    window.draw(flagSprite);
                                }
                            }
                        }
                    }

                    window.draw(happyFaceButtonSprite);
                    window.draw(debugButtonSprite);
                    window.draw(leaderboardButtonSprite);



                    if(wasResetClicked){
                        //cout<<"I entered if statement"<<endl;
                        if(wasPauseClicked){
                            //cout<<"I entered somewhere i shouldnt"<<endl;
                            if(wasResumeClicked){
                                timer.start();
                                wasResetClicked = false;
                                wasPauseClicked = false;
                                wasResumeClicked = false;
                            }
                        }
                        else{
                            //cout<<"I entered correctly"<<endl;
                            timer.start();
                            wasResetClicked = false;
                        }

                    }

                    if(debugMode || lost){
                        for(int i = 0; i < rows; i++){
                            for(int j = 0; j < columns; j++){
                                float xShift = j * revealedTileSpriteBounds.width;
                                float yShift = i * revealedTileSpriteBounds.height;
                                if(Tiles[i][j]->isBomb){
                                    mineSprite.setPosition((xShift), yShift);
                                    window.draw(mineSprite);
                                }
                            }
                        }
                    }

                    if(lost){
                        window.draw(loseFaceSprite);
                        abilityToRightClick = false;
                    }

                    if(didPlayerWin()){
                        window.draw(winFaceSprite);
                        allowedToClickTiles = false;
                        allowedToClickDebug = true;
                        allowedToClickPausePlay = false;
                    }
                    else if(!lost){
                        allowedToClickTiles = true;
                        allowedToClickPausePlay = true;
                        abilityToRightClick = true;
                        allowedToClickDebug = true;
                    }
                    //didPlayerWin();

                    if(wasPauseClicked){
                        //cout<<"I entered just pause"<<endl;
                        allowedToClickTiles = false;
                        if(wasResumeClicked){
                            timer.resume();
                            wasPauseClicked = false;
                            wasResumeClicked = false;
                            allowedToClickTiles = true;
                        }
                    }

                    if (displayPause) {
                        window.draw(pauseButtonSprite);
                    } else {
                        window.draw(playButtonSprite);
                    }

                    if (flagsAvailable < 0) {
                        counterSprite.setTextureRect(IntRect(21 * 10, 0, 21, 32));
                        counterSprite.setPosition((12), (32 * (rows + 0.5) + 16));
                        window.draw(counterSprite);
                    }

                    if(wasPauseClicked){
                        //cout<<"I am in here cause i clicked leader"<<endl;
                        for (int i = 0; i < Tiles.size(); i++) {
                            for (int j = 0; j < Tiles[i].size(); j++) {
                                float xShift = j * revealedTileSpriteBounds.width;
                                float yShift = i * revealedTileSpriteBounds.height;

                                revealedTileSprite.setPosition(xShift, yShift);
                                window.draw(revealedTileSprite);
                            }
                        }
                    }

                    if(needToDisplayLdrBoard){
                        //cout<<"I am in leader, printing revealed" <<endl;

                        //cout<<"entering Leaderboard print"<<endl;
                        for (int i = 0; i < Tiles.size(); i++) {
                            for (int j = 0; j < Tiles[i].size(); j++) {
                                float xShift = j * revealedTileSpriteBounds.width;
                                float yShift = i * revealedTileSpriteBounds.height;

                                revealedTileSprite.setPosition(xShift, yShift);
                                window.draw(revealedTileSprite);
                            }
                        }
                        //cout<<"I finished displaying tiles"<<endl;
                        vector<int> flagDigits = counterDigits(flagsAvailable);
                        for (int i = 0; i < flagDigits.size(); i++) {
                            counterSprite.setTextureRect(IntRect(21 * flagDigits[i], 0, 21, 32));
                            counterSprite.setPosition(33 + (21 * i), 32 * (rows + 0.5) + 16);
                            window.draw(counterSprite);
                        }

                        //Display Time
                        int elapsedTime = timer.getElapsedTime();
                        vector<int> minTimeDigits = minCounterDigits(elapsedTime);
                        vector<int> secTimeDigits = secondCounterDigits(elapsedTime);

                        for (int i = 0; i < minTimeDigits.size(); i++) {
                            counterSprite.setTextureRect(IntRect(21 * minTimeDigits[i], 0, 21, 32));
                            counterSprite.setPosition((columns * 32) - 97 + 21 * i, 32 * (rows + 0.5) + 16);
                            window.draw(counterSprite);
                        }

                        for (int i = 0; i < secTimeDigits.size(); i++) {
                            counterSprite.setTextureRect(IntRect(21 * secTimeDigits[i], 0, 21, 32));
                            counterSprite.setPosition((columns * 32) - 54 + 21 * i, 32 * (rows + 0.5) + 16);
                            window.draw(counterSprite);
                        }

                        window.display();
                        displayLeaderboard(columns, rows);

                        needToDisplayLdrBoard = false;
                    }
                    //cout<<"I still print even though in different while loop"<<endl;

                    //printing flag digits
                    vector<int> flagDigits = counterDigits(flagsAvailable);
                    for (int i = 0; i < flagDigits.size(); i++) {
                        counterSprite.setTextureRect(IntRect(21 * flagDigits[i], 0, 21, 32));
                        counterSprite.setPosition(33 + (21 * i), 32 * (rows + 0.5) + 16);
                        window.draw(counterSprite);
                    }

                    //Display Time
                    int elapsedTime = timer.getElapsedTime();
                    vector<int> minTimeDigits = minCounterDigits(elapsedTime);
                    vector<int> secTimeDigits = secondCounterDigits(elapsedTime);

                    for (int i = 0; i < minTimeDigits.size(); i++) {
                        counterSprite.setTextureRect(IntRect(21 * minTimeDigits[i], 0, 21, 32));
                        counterSprite.setPosition((columns * 32) - 97 + 21 * i, 32 * (rows + 0.5) + 16);
                        window.draw(counterSprite);
                    }

                    for (int i = 0; i < secTimeDigits.size(); i++) {
                        counterSprite.setTextureRect(IntRect(21 * secTimeDigits[i], 0, 21, 32));
                        counterSprite.setPosition((columns * 32) - 54 + 21 * i, 32 * (rows + 0.5) + 16);
                        window.draw(counterSprite);
                    }

                    //cout<<"(T/F) Player has Won: ";
                    //cout << (didPlayerWin()) << endl;
                    if(didPlayerWin() && !(didIShowNewLeaderboard)){
                        cout<<"THIS IS MY PERFECT VICTORY L, I WIN!"<<endl;
                        window.draw(winFaceSprite);
                        timer.stop();
                        updateLeaderboard(timer.getElapsedTime());
                        for(int i = 0; i < rows; i++){
                            for(int j = 0; j < columns; j++){
                                if(Tiles[i][j]->isBomb){
                                    float xShift = j * revealedTileSpriteBounds.width;
                                    float yShift = i * revealedTileSpriteBounds.height;

                                    flagSprite.setPosition(xShift, yShift);
                                    window.draw(flagSprite);
                                }
                            }
                        }
                        window.display();
                        didIShowNewLeaderboard = true;
                        displayLeaderboard(columns, rows);
                        //cout<<timer.getElapsedTime()<<endl;
                    }



                    window.display();
                }
            }
        }
};
