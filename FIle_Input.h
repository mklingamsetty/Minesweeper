#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

//17948 section number

class FileReader {
    private:
        map<string, string> leaderboard;
        int columns;
        int rows;
        int mines;

    public:
        void readFile(string filename) {
            if (filename == "files/leaderboard.txt") {
                ifstream MyFile(filename);
                while (!MyFile.eof()) {
                    string score = "";
                    string name = "";
                    getline(MyFile, score, ',');
                    getline(MyFile, name, '\n');

                    leaderboard.emplace(score, name);
                }
                MyFile.close();
            } else {
                string strColumns;
                string strRows;
                string strMines;

                ifstream MyFile(filename);
                getline(MyFile, strColumns);
                getline(MyFile, strRows);
                getline(MyFile, strMines);

                columns = stoi(strColumns);
                rows = stoi(strRows);
                mines = stoi(strMines);

                MyFile.close();
            }
        }

        int getColumns() {
            return columns;
        }

        int getRows() {
            return rows;
        }

        int getMines() {
            return mines;
        }

        map<string, string> getLeaderboard() {
            return leaderboard;
        }

        void setLeaderboard(map<string, string> newLeaderboard){
            leaderboard = newLeaderboard;
        }

        void writeLeaderboard(){
            ofstream newLeaderboardFile("leaderboard.txt");
            for(auto i = leaderboard.begin(); i != leaderboard.end(); i++){
                newLeaderboardFile << i->first << ", " << i->second <<endl;
            }
        };
};