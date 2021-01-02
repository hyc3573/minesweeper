#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include <string>
#include <functional>
#include "ConnectionIii-Rj3W.hpp"

#define SWIDTH 250
#define SHEIGHT 275

#define TWIDTH 25
#define THEIGHT 25

#define BWIDTH 10
#define BHEIGHT 10

#define MINEPERCENT 5

#define DEBUG

using namespace std;
using namespace sf;

typedef enum Type { MINE = 0, BLANK } Type;

class Square
{
private:
    int X;
    int Y;
    bool mine;
    bool marked;
    bool opened;
    String text;
    vector<vector<Type>>& board;
    bool edge;
    int number;

public:
    Square(vector<vector<Type>>& board, int X, int Y) : board(board), X(X), Y(X), mine(board[X][Y] == MINE), marked(false), opened(false) 
    {
        text = L"";
        edge = X == 0 || Y == 0 || X == BWIDTH + 1 || Y == BHEIGHT + 1;
        mine = !edge && mine;
        if (!edge)
        {
            number = 0;
            number += board[X - 1][Y - 1] == MINE;
            number += board[X][Y - 1] == MINE;
            number += board[X + 1][Y - 1] == MINE;
            number += board[X - 1][Y] == MINE;
            number += board[X + 1][Y] == MINE;
            number += board[X - 1][Y + 1] == MINE;
            number += board[X][Y + 1] == MINE;
            number += board[X + 1][Y + 1] == MINE;
        }
    }

    Square(vector<vector<Type>>& board) : board(board), X(0), Y(0), mine(false), marked(false), opened(false), edge(false), number(0), text("") {};

    Square& operator=(const Square& rhs)
    {
        board = rhs.board;
        X = rhs.X;
        Y = rhs.Y;
        mine = rhs.mine;
        marked = rhs.marked;
        opened = rhs.opened;
        edge = rhs.edge;
        number = rhs.number;
        text = rhs.text;
        return *this;
    }

    bool isMine()
    {
        return mine;
    }

    bool isMarked()
    {
        return marked;
    }

    bool isOpened()
    {
        return opened;
    }

    void open()
    {
        opened = true;
        if (!mine)
        {
            text = to_string(number);
            if (!number)
            {
                text = L"";
            }
        }
        else
        {
            text = L"M";
        }
    }

    void mark()
    {
        marked = !marked;
        text = marked? L"P" : L"";
    }

    String getString()
    {
        return text;
    }

    bool isEdge()
    {
        return edge;
    }

    int getNum()
    {
        return number;
    }
};

int main()
{
    unsigned int t = GetTickCount64();
    srand(t);

    RenderWindow window(VideoMode(SWIDTH, SHEIGHT), L"크고♂아름다운♂게이ㅁ");

    Event event;

    Clock clock;

    Font font;
    if (!font.loadFromMemory(ConnectionIii_Rj3W_otf, ConnectionIii_Rj3W_otf_len))
    {
        cout << "error!" << endl;
    }

    vector<vector<Type>> board(BWIDTH + 2, vector<Type>(BHEIGHT + 2, BLANK));

    vector<vector<Square>> squares(BWIDTH + 2, vector<Square>(BHEIGHT + 2, Square(board)));

    vector<vector<RectangleShape>> boardShape;
    vector<vector<Text>> texts;


    bool leftMouseClicked;
    bool rightMouseClicked;
    Vector2f mousePos;

    bool cleared;

    auto reset = [&]()
    {
        for (int i = 0; i < BWIDTH + 2;i++)
        {
            for (int j = 0;j < BHEIGHT + 2;j++)
            {
                board[i][j] = static_cast<Type>(!(rand() % 100 < MINEPERCENT));
            }
        }

        for (int i = 0; i < BWIDTH + 2;i++)
        {
            for (int j = 0;j < BHEIGHT + 2;j++)
            {
                squares[i][j] = Square(board, i, j);
            }
        }

        for (int i = 0; i < BWIDTH;i++)
        {
            boardShape.push_back(vector<RectangleShape>());
            texts.push_back(vector<Text>());
            for (int j = 0; j < BHEIGHT; j++)
            {
                boardShape[i].push_back(RectangleShape(Vector2f(TWIDTH, THEIGHT)));
                boardShape[i][j].setFillColor(Color::White);
                boardShape[i][j].setOutlineColor(Color::Yellow);
                boardShape[i][j].setPosition(i * TWIDTH, j * THEIGHT);
                boardShape[i][j].setOutlineThickness(1);

                texts[i].push_back(Text());
                texts[i][j].setFont(font);
                texts[i][j].setString(squares[i + 1][j + 1].getString());
                texts[i][j].setFillColor(Color::Red);
                texts[i][j].setPosition(i * TWIDTH - texts[i][j].getLocalBounds().width / 2, j * THEIGHT - texts[i][j].getLocalBounds().height / 2);
                texts[i][j].setCharacterSize(THEIGHT);
                texts[i][j].setOrigin(texts[i][j].getLocalBounds().width / 2, texts[i][j].getLocalBounds().height / 2);
            }
        }

        leftMouseClicked = false;
        rightMouseClicked = false;
        cleared = false;
    };

    function<void(int,int)> open = [&](int X, int Y)
    {
        squares[X][Y].open();
        boardShape[X - 1][Y - 1].setFillColor(Color::Blue);

        if (squares[X][Y].getNum() == 0)
        {
            if (!squares[X - 1][Y].isOpened() && !(board[X - 1][Y] == MINE) && !squares[X - 1][Y].isEdge()) open(X - 1, Y);
            if (!squares[X][Y - 1].isOpened() && !(board[X][Y - 1] == MINE) && !squares[X][Y - 1].isEdge()) open(X, Y - 1);
            if (!squares[X + 1][Y].isOpened() && !(board[X + 1][Y] == MINE) && !squares[X + 1][Y].isEdge()) open(X + 1, Y);
            if (!squares[X][Y + 1].isOpened() && !(board[X][Y + 1] == MINE) && !squares[X][Y + 1].isEdge()) open(X, Y + 1);
        }
        else
        {
            if (!squares[X - 1][Y].isOpened() && !(board[X - 1][Y] == MINE) && (!squares[X - 1][Y].isEdge() && !squares[X - 1][Y].getNum())) open(X - 1, Y);
            if (!squares[X][Y - 1].isOpened() && !(board[X][Y - 1] == MINE) && (!squares[X][Y - 1].isEdge() && !squares[X][Y - 1].getNum())) open(X, Y - 1);
            if (!squares[X + 1][Y].isOpened() && !(board[X + 1][Y] == MINE) && (!squares[X + 1][Y].isEdge() && !squares[X + 1][Y].getNum())) open(X + 1, Y);
            if (!squares[X][Y + 1].isOpened() && !(board[X][Y + 1] == MINE) && (!squares[X][Y + 1].isEdge() && !squares[X][Y + 1].getNum())) open(X, Y + 1);
        }
    };

    reset();

    while (window.isOpen()) 
    {
        leftMouseClicked = false;
        rightMouseClicked = false;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;

            case Event::MouseButtonPressed:
                switch (event.mouseButton.button)
                {
                case Mouse::Left:
                    leftMouseClicked = true;
                    break;
                case Mouse::Right:
                    rightMouseClicked = true;
                    break;
                }
                break;

            case Event::MouseMoved:
                mousePos = Vector2f(event.mouseMove.x, event.mouseMove.y);
            }
        }

        cleared = true;
        for (int i = 1; i < BWIDTH + 1;i++)
        {
            for (int j = 1;j < BHEIGHT + 1;j++)
            {
                if (leftMouseClicked && boardShape[i - 1][j - 1].getGlobalBounds().contains(mousePos))
                {
                    if (board[i][j] == MINE)
                    {
                        reset();
                        break;
                    }
                    open(i, j);
                }
                if (rightMouseClicked && boardShape[i - 1][j - 1].getGlobalBounds().contains(mousePos))
                {
                    squares[i][j].mark();
                }

                if (board[i][j] == MINE && !squares[i][j].isMarked())
                {
                    cleared = false;
                }
                if (board[i][j] != MINE && squares[i][j].isMarked())
                {
                    cleared = false;
                }
            }
        }
        if (cleared)
        {
            reset();
        }

        window.clear();

        for (int i = 1; i < BWIDTH + 1;i++)
        {
            for (int j = 1;j < BHEIGHT + 1;j++)
            {
                texts[i - 1][j - 1].setString(squares[i][j].getString());

                window.draw(boardShape[i - 1][j - 1]);
                
                window.draw(texts[i - 1][j - 1]);
            }
        }

        window.display();
    }
    return 0;
}