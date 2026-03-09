#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
protected:

    sf::Sprite sprite;
    sf::Texture texture;

    float speed;

public:
    int health;
    int maxHealth;

    Entity()
    {
        speed = 2.0f;
        health = 100;
        maxHealth = 100;
    }

    virtual ~Entity(){}

    virtual void update() = 0;

    virtual void draw(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }

    void setPosition(float x, float y)
    {
        sprite.setPosition(x,y);
    }

    sf::Vector2f getPosition()
    {
        return sprite.getPosition();
    }

    sf::Sprite& getSprite()
    {
        return sprite;
    }
    
    int getHealth() {
        return health;
    }

};