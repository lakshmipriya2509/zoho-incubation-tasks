#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
using namespace std;
void board() {
    for (int i = 0; i < 22; i++) {
        cout << "#";
    }cout << endl;
}
void swap(string* a, string* b) {
    string temp;
    temp = *a;
    *a = *b;
    *b = *a;
}
void Shuffle(vector<string>& cards) {
    auto range = default_random_engine{};
    shuffle(begin(cards), end(cards), range);
}
void Print(vector<string> cards) {
    int y = 0;
    board();
    for (string x : cards) {
        y++;
        cout << "# ";
        if (x.length() < 2) { cout << " "; }
        cout << x << " ";
        if (y == 4 || y == 8 || y == 12) {
            cout << " #" << endl;
            for (int i = 0; i < 22; i++) {
                cout << "#";
            }cout << endl;
        }
    }
    cout << " #" << endl;
    board();
    cout << "\n\n";
}
void Reset(vector<string>& cards) {
    for (int y = 1; y < 16; y++) {
        string s = to_string(y);
        cards.push_back(s);
    }
    cards.push_back(" ");
}
int index(vector<string>& cards) {
    auto it = find(cards.begin(), cards.end(), " ");
    int index = distance(cards.begin(), it);
    return index;
}
int MoveLogic(int x, int side, char move) {
    switch (move) {
    case 'w':
        if (x < 12) {
            side = x + 4;
        }
        break;
    case 's':
        if (x > 3) {
            side = x - 4;
        }
        break;
    case 'a':
        if (x != 3 && x != 7 && x != 11 && x != 15) {
            side = x + 1;
        }
        break;
    case 'd':
        if (x != 0 && x != 4 && x != 8 && x != 12) {
            side = x - 1;
        }
        break;
    default:
        side = x;
    }
    return side;
}
void logic(vector<string>& cards, char move, int& moves) {

    int x = index(cards);
    int side = x;
    side = MoveLogic(x, side, move);
    if (x == side) {
        moves--;
    }
    swap(cards[x], cards[side]);

}
void GO(vector<string>& cards, bool& GameOver) {
    GameOver = true;
    for (int x = 0; x < 15; x++) {
        if (cards[x] != to_string(x + 1))
            GameOver = false;
    }
}
int main() {
    bool GameOver = false;
    char move = 'x';
    int moves = 0;
    vector<string>c;
    Reset(c);
    Shuffle(c);
    while (!GameOver) {
        cout << "\nmoves : " << moves++ << "\n\n";
        Print(c);
        cin >> move;
        logic(c, move, moves);
        GO(c, GameOver);
    }
    Print(c);
    cout << "Grate Job!\n Your Finish The Game With Only : " << moves << " : Moves!\n\n";



    return 0;
}
