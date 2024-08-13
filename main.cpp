#include <iostream>
#include "File_Input.h"
#include "GameWindows.h"

int main() {
    GameWindows gameScreen;
    FileReader myFiles;
    myFiles.readFile("files/config.cfg");
    myFiles.readFile("files/leaderboard.txt");
     cout<< "Columns: " << myFiles.getColumns() << "; Rows: " << myFiles.getRows()<<endl;

    RenderWindow window(VideoMode((myFiles.getColumns() * 32), ((myFiles.getRows() * 32) + 100)),
                        "Welcome Window", Style::Close);

    Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cerr << "Error loading font\n";
        return -1;
    }

    Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setStyle(Text::Bold | Text::Underlined);
    welcomeText.setFillColor(Color::White);
    welcomeText.setPosition(
            (window.getSize().x - welcomeText.getLocalBounds().width) / 2,
            window.getSize().y / 2 - 150
    );

    Text promptText("Enter your name:", font, 20);
    promptText.setFillColor(Color::White);
    promptText.setPosition(
            (window.getSize().x - promptText.getLocalBounds().width) / 2,
            window.getSize().y / 2 - 75
    );

    string playerName;
    Text nameText("", font, 18);
    nameText.setFillColor(Color::Yellow);
    nameText.setPosition(
            (window.getSize().x - nameText.getLocalBounds().width) / 2,
            window.getSize().y / 2 - 45
    );

    //map<string, string> leaderboard = myFiles.getLeaderboard();

    bool showCursor = true;
    gameScreen.leaderboardSetup(myFiles.getLeaderboard(), font);
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!playerName.empty()) {
                        playerName.pop_back();
                    }
                } else if (event.text.unicode == 13) { // Enter
                    if (!playerName.empty()) {

                        for (auto& c : playerName) {
                            c = toupper(c);
                        }
                        window.close(); // Close welcome window
                        showCursor = false;
                        RenderWindow gameWin(VideoMode((myFiles.getColumns() * 32), ((myFiles.getRows() * 32) + 100)),
                         "Minesweeper", Style::Close);

                        gameScreen.gameWindow(gameWin, myFiles.getRows(), myFiles.getColumns(), myFiles.getMines(), playerName);
                    }
                } else if (playerName.size() < 10 && isalpha(event.text.unicode)) {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }

        string displayedText;
        if (showCursor) {
            for(int i = 0; i < playerName.length(); i++){
                if(i == 0){
                    displayedText += toupper(playerName[i]);
                }
                else{
                    displayedText += tolower(playerName[i]);
                }
            }
            displayedText += '|';
        }

        nameText.setString(displayedText);
        nameText.setPosition(
                (window.getSize().x - nameText.getLocalBounds().width) / 2,
                window.getSize().y / 2 - 45
        );

        window.clear(Color::Blue);
        window.draw(welcomeText);
        window.draw(promptText);
        window.draw(nameText);
        window.display();
    }

    return 0;
}
