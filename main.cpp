#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <Windows.h>

#define SWIDTH 250
#define SHEIGHT 275

#define TWIDTH 25
#define THEIGHT 25

using namespace std;
using namespace sf;

int main()
{
    unsigned int t = GetTickCount64();
    srand(t);

    RenderWindow window(VideoMode(SWIDTH, SHEIGHT), L"크고♂아름다운♂게이ㅁ");

    Event event;

    Clock clock;

    RectangleShape rect(Vector2f(TWIDTH, THEIGHT));
    rect.setFillColor(Color::White);

    while (window.isOpen()) 
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            }
        }
        window.clear();

        window.draw(rect);

        window.display();
    }
    return 0;
}