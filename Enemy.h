#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Enemy : public Entity
{
public:
    enum EType { ORC, SKELETON, KNIGHT, ROGUE, WIZZARD };
    enum EState { IDLE, RUNNING, HIT, DEAD };

private:
    EType mobType;
    sf::Vector2f targetPos;

    sf::Texture tIdle, tRun, tHit, tDeath;

    EState currentState;

    sf::Clock animClock;
    int animFrame;
    
    bool isDeadFinished;

public:

    Enemy(float x = 500.f, float y = 500.f, EType type = ORC);

    void update() override;

    void draw(sf::RenderWindow& window) override;

    void takeDamage(int dmg);

    bool isDead();
    bool isAnimFinished() { return isDeadFinished; }

    void setTarget(sf::Vector2f target);
    sf::Vector2f getPosition() const;
};