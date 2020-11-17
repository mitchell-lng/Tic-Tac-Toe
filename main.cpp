#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <sstream>

using std::string;
using std::cout;
using std::cin;
using std::map;
using std::vector;

enum BotType {Random, Strategic};

class Board {
    public:
        Board(int height, int width)
        {
            grid[0] = width;
            grid[1] = height;
            size = grid[0] * grid[1] + 1;
            for (int i = 1; i < size; i++)
            {
                board->insert( std::pair<int, char>(i, ' ') );
            }
        }

        void printBoard()
        {
            cout << "\n";
            for (int i = 1; i < size; i++) {
                cout << " " << board->at(i) << " ";
                if (i != size - 1) {
                    if (i % grid[0] != 0) cout << "|";
                    else {
                        int underscoreSize = grid[0] * 4 - 1;
                        string out(underscoreSize, '-');
                        cout << "\n" << out << "\n";
                    }
                }
            }

            cout << "\n\n";
        }

        void setSquare(int boardID, char character) {
            board->at(boardID) = character;
        }

        bool getSquare(int boardID) {
            // False = taken; true = available
            return board->at(boardID) == ' ';
        }

        bool isWhole(int number) {
            if (floor(number) == number) return true;
            return false;
        }

        int canWin() {
            // Check Rows
            for (int i = 0; i < grid[0]; i++) {
                char character = 'L';

                for (int j = 0; j < grid[1]; j++) {
                    int number = j + (i * grid[0]) + 1;
                    if (!getSquare(number)) character = board->at(number);
                }

                if (character != 'L') {
                    int score = 0;
                    int index;
                    for (int j = 0; j < grid[1]; j++) {
                        int number = j + (i * grid[0]) + 1;
                        if (board->at(number) == character) {
                            score++;
                        } else {
                            index = number;
                        }
                    }

                    if (score == grid[1] - 1  && getSquare(index)) return index;
                }
            }

            // Checks Columns
            for (int i = 0; i < grid[1]; i++) {
                char character = 'L';

                for (int j = 0; j < grid[0]; j++) {
                    int number = i + (j * grid[0]) + 1;
                    if (!getSquare(number)) character = board->at(number);
                }

                if (character != 'L') {
                    int score = 0;
                    int index;
                    for (int j = 0; j < grid[0]; j++) {
                        int number = i + (j * grid[0]) + 1;
                        if (board->at(number) == character) {
                            score++;
                        } else {
                            index = number;
                        }
                    }

                    if (score == grid[1] - 1  && getSquare(index)) return index;
                }
            }

            // Check Diagonals
            if (grid[0] == grid[1]) {
                for (int i = 0; i < 2; i++) {
                    char character = 'L';
                    int incrementVal;
                    int startVal;

                    if (i == 0) {
                        startVal = 1;
                        incrementVal = grid[0] + 1;
                    } else {
                        startVal = grid[0];
                        incrementVal = grid[0] - 1;
                    }

                    for (int j = 0; j < grid[0]; j++) {
                        int number = startVal + (j * incrementVal);
                        if (!getSquare(number)) character = board->at(number);
                    }

                    if (character != 'L') {
                        int score = 0;
                        int index;
                        for (int j = 0; j < grid[0]; j++) {
                            int number = startVal + (j * incrementVal);
                            if (board->at(number) == character) {
                                score++;
                            } else {
                                index = number;
                            }
                        }

                        if (score == grid[1] - 1  && getSquare(index)) return index;
                    }
                }
            }

            return -1;
        }

        int middleOutside() {
            vector<int> numbers;
            for (int i = 0; i < grid[0]; i++) {
                for (int j = 0; i < grid[1]; i++) {
                    int number = (i * grid[0]) + j + 1;

                    if (i == 0 || i == (grid[0] - 1)) {
                        if (j != 0 && j != grid[1] - 1) {
                            numbers.push_back(number);
                        }
                    } else {
                        if (j == 0 || j == grid[1] - 1) {
                            numbers.push_back(number);
                        }
                    }
                }
            }

            std::random_shuffle( numbers.begin(), numbers.end() );

            for (int i: numbers) {
                if (getSquare(i)) return i;
            }

            return -1;
        }

        int middle() {
            int size = grid[0] * grid[1];
            
            float directMiddle = (size - 1) / 2 + 1;
            if (isWhole(directMiddle) && getSquare(directMiddle)) {
                return directMiddle;
            }

            return -1;
        }

        int corner() {
            vector<int> corners = {1, grid[0], (grid[1] * (grid[0] - 1)) + 1, grid[1] * grid[0]};

            for (int i: corners) {
                if (getSquare(i))
                    return i;
            }

            return -1;
        }

        bool win()
        {
            // Searches Rows for wins
            for (int i = 0; i < grid[0]; i++)
            {
                bool returnVal = true;
                int change = i * grid[0];

                char firstChar = board->at(change + 1);
                if (firstChar == ' ') continue;
                for (int j = 1; j < grid[1] + 1; j++) 
                {
                    if (board->at(change + j) != firstChar)
                    {
                        returnVal = false;
                    }
                }

                if (returnVal)
                {
                    cout << firstChar << " won!\n";
                    return true;
                }
            }

            // Searches Columns for wins
            for (int i = 1; i < grid[1] + 1; i++)
            {
                bool returnVal = true;
                int change;

                char firstChar = board->at(i);
                if (firstChar == ' ') continue;
                for (int j = 1; j < grid[0]; j++) 
                {
                    change = j * grid[0];
                    if (board->at(change + i) != firstChar)
                    {
                        returnVal = false;
                    }
                }

                if (returnVal)
                {
                    cout << firstChar << " won!\n";
                    return true;
                }
            }

            // Checks to make sure it is a square
            if (grid[0] == grid[1])
            {
                // Checks the two diagonals
                int startVal;
                int change;
                int incrementVal;
                bool returnVal;
                for (int i = 0; i < 2; i++)
                {
                    returnVal = true;

                    if (i == 0) {
                        startVal = 1;
                        incrementVal = grid[0] + 1;
                    } else {
                        startVal = grid[0];
                        incrementVal = grid[0] - 1;
                    }

                    char firstChar = board->at(startVal);
                    if (firstChar == ' ') continue;

                    for (int j = 1; j < grid[0]; j++)
                    {
                        change = j * incrementVal;
                        if (firstChar != board->at(change + startVal)) returnVal = false;
                    }

                    if (returnVal)
                    {
                        cout << firstChar << " won!\n";
                        return true;
                    }
                }
            }

            return false;
        }

        bool boardFull()
        {
            
            for (int i = 1; i < size; i++)
            {
                if (board->at(i) == ' ') return false;
            }

            return true;
        }

        int getBoardSize() const { return size; }

    private:
        std::unique_ptr<map<int, char>> board = std::make_unique<map<int, char>>() ;

        // grid must be the same number for the diagonal function to work correctly
        int grid[2] = {3, 3};
        int size;
};

class Bot {
    public:
        Bot(int size, BotType type)
        {
            cout << "Bot Created!\n";
            _size = size - 1;
            _type = type;
        }

        int pick(Board &b) {
            if (_type == Strategic) { return strategicPick(b); }
            return getRandomPick();
        }

        int getRandomPick()
        {
            return (rand() % _size) + 1;
        }

        int strategicPick(Board &board)
        {
            int cw = board.canWin();
            if (cw != -1) {
                return cw;
            } else {
                int middle = board.middle();
                if (middle != -1) {
                    return middle;
                } else {
                    if (!singleCorner)
                    {
                        int corner = board.corner();
                        singleCorner = true;
                        if (corner != -1) return corner;
                    }

                    int middleout = board.middleOutside();
                    if (middleout != -1) {
                        return middleout;
                    } else {
                        return getRandomPick();
                    }
                }
            }

            return 0;
        }
    private:
        int _size;
        BotType _type;

        bool singleCorner = false;

};

int getInput(int size) {
    int input = 0;
    while (!(input > 0 && input < size))
    {
        cout << "Enter a number 1-" << size - 1 << ": ";
        cin >> input;
    }

    return input;
}

void init() {
    vector<int> widthHeight;
    BotType botType;

    static map<std::string, BotType> const table = { {"Random", BotType::Random}, {"Strategic", BotType::Strategic} };

    std::ifstream file("inputs.txt");
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        string key, value;

        iss >> key >> value;

        if (key == "Size") {
            std::istringstream ss(value);
            std::string token;

            while(std::getline(ss, token, ',')) {
                widthHeight.push_back(std::stoi(token));
            }
        }

        if (key == "Bot-Type") {
            auto it = table.find(value);
            if (it != table.end())
                botType = it->second;
        }
    }

    srand(time(NULL));

    Board board(widthHeight.at(0), widthHeight.at(1));
    Bot bot(board.getBoardSize(), botType);

    int turn = 0;
    char currentChar = 'X';
    int val = 0;

    // Game Loop
    while(!board.win() && !board.boardFull()) {

        if (turn % 2 == 0) {
            board.printBoard();
            val = getInput(board.getBoardSize());
            currentChar = 'X';
        }
        else {
            val = bot.pick(board);
            currentChar = 'O';
        }

        if (board.getSquare(val)) {
            board.setSquare(val, currentChar);
            turn++;
        } else {
            if (turn % 2 == 0) {
                cout << "Find a square that isn't already occupied\n";
            }
        }
    }

    board.printBoard();
}

int main() {
    string play = "true";
    while (!(play == "exit" || play == "no" || play == "NO" || play == "EXIT" || play == "Exit" || play == "No"))
    {
        init();
        cout << "Play Again? (Type Exit or no to leave) ";
        cin >> play;
    }

    return 0;
}