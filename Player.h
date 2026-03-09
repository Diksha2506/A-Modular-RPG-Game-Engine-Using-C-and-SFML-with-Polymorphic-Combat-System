#pragma once
#include "Entity.h"

class Player : public Entity
{

public:

    enum State { IDLE, WALKING, ATTACKING, DRINKING, EATING, HARVESTING, CRUSHING };
    enum Direction { DOWN, UP, SIDE };

private:

    int attackPower;

    sf::Texture tWalkDown, tWalkUp, tWalkSide;
    sf::Texture tIdleDown, tIdleUp, tIdleSide;
    sf::Texture tSliceDown, tSliceUp, tSliceSide;
    sf::Texture tCollectDown, tCollectUp, tCollectSide;
    sf::Texture tCrushDown, tCrushUp, tCrushSide;

    State currentState;
    Direction currentDir;

    sf::Clock animClock;
    int animFrame;

    bool facingLeft;

struct Inventory {
    int wood = 0;
    int food = 0;
    int water = 0;
    int stone = 0;
    int weapons = 1;
    int armor = 0;
    int weaponLevel = 1;
    int metal = 0;
};

public:
    Inventory inv;

    Player();

    void handleInput();

    void update() override;

    int getAttackPower();

    void triggerAttack();
    void triggerEat();
    void triggerDrink();
    void triggerHarvest();
    void triggerCrush();
};