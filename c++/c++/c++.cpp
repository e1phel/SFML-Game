#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;
using namespace sf;

class base
{
protected:
    float hp, atk_dmg,posx, posy;
    
    virtual void draw() {};
    virtual void move(RenderWindow& window) {};
    virtual void attack() {};
};
class zone
{
protected:
    float tx, ty, by, rx, tall_bar_y, lx, long_bar_x;
    RectangleShape top_bar, bottom_bar, left_bar, right_bar;
public:
    zone()
    {
        //top
        long_bar_x = 20; ty = (720 / 3);
        top_bar.setSize(Vector2f(1220.0, 30.0));
        top_bar.setFillColor(Color::Cyan);
        top_bar.setPosition(Vector2f(long_bar_x, ty));
        //bottom
        by = 680;
        bottom_bar.setSize(Vector2f(1220.0, 30.0));
        bottom_bar.setFillColor(Color::Cyan);
        bottom_bar.setPosition(Vector2f(long_bar_x, by));
        //right
        rx = 1220; tall_bar_y = 720 / 3;
        right_bar.setSize(Vector2f(30, (720 / 1.54)));
        right_bar.setFillColor(Color::Cyan);
        right_bar.setPosition(Vector2f(rx, tall_bar_y));
        //left
        lx = 20;
        left_bar.setSize(Vector2f(30, (720 / 1.54)));
        left_bar.setFillColor(Color::Cyan);
        left_bar.setPosition(Vector2f(lx, tall_bar_y));
    }
    void draw_zone(RenderWindow& window)
    {
        window.draw(top_bar);
        window.draw(left_bar);
        window.draw(right_bar);
        window.draw(bottom_bar);
    }
    double get_zone_top_limit() { return ty; }
    double get_zone_left_limit() { return lx; }
    double get_zone_right_limit() { return rx; }
    double get_zone_bottom_limit() { return by; }
};
class projectiles :public base
{
protected:
    CircleShape big;
    ConvexShape small;
    zone z;
    float px, py,reset;
public:
    projectiles(float x, float y):big(25.f, 3),px(x),py(y)
    {
        big.setFillColor(Color::Yellow);
        big.setRotation(degrees(180.f));
        reset = 770;
    }
    void attack(RenderWindow& window)
    {
        if (py < z.get_zone_bottom_limit())
        {
            py += 4;
        }
        else
        {
            py = reset;
        }
        big.setPosition(Vector2f(px, py));
        window.draw(big);
    }
};
class enemy :public base
{
protected:
    RectangleShape boss;
    bool move_left = false,attack = false; zone z;
    
public:
    enemy()
    {
        posx = 1280 / 2;
        posy = 720 / 10;
        boss.setSize(Vector2f(100, 100));
        boss.setFillColor(Color::Red);
    }
    void move(RenderWindow& window)override
    {
        projectiles p{ posx+76,posy+150 };
        if(move_left == false)
        {
            posx += 3.5;
            if (posx >= z.get_zone_right_limit())
            {
                move_left = true;
            }

        }
        else
        {
            posx -= 3.5;
            if (posx <= z.get_zone_left_limit())
            {
                move_left = false;
            }
        }
        boss.setPosition(Vector2f(posx, posy));
        window.draw(boss);
        cout << posx << "|" << posy << endl;
    }
    float get_boss_x() { return posx; }
    float get_boss_y() { return posy; }
};
class player :public base
{
protected:
    RectangleShape PL;
    zone z;
    int speed = 4;
public:
    player()
    {
        posx = (1280/2)-2;
        posy = 450;
        PL.setSize(Vector2f(50, 50));
        PL.setFillColor(Color::Magenta);
    }
    void move(RenderWindow& window)override
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            if (posy > (z.get_zone_top_limit()+30))
            {
                posy -= speed;
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            if (posy < (z.get_zone_bottom_limit() - 50))
            {
                posy += speed;
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            if (posx < z.get_zone_right_limit() - 50)
            {
                posx += speed;
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            if (posx > z.get_zone_left_limit() + 30)
            {
                posx -= speed;
            }
        }
        PL.setPosition(Vector2f(posx, posy));
        window.draw(PL);
      
    }
};
int main()
{
    RenderWindow window(VideoMode({ 1280, 720 }), "SFML works!");
    enemy e; zone z; player p; projectiles v(600, 300);
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        window.setFramerateLimit(60);
        window.clear();
        e.move(window);
        z.draw_zone(window);
        p.move(window);
        window.display();
    }
}