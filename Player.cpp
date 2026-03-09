#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Player::Player()
{
    health = 100;
    maxHealth = 100;
    speed = 3.5f;
    attackPower = 20;
    currentState = IDLE;
    currentDir = DOWN;
    animFrame = 0;
    facingLeft = false;

    // Load new assets
    // Load all directional sheets
    tIdleDown.loadFromFile("assets_new/Idle_Down-Sheet.png");
    tIdleUp.loadFromFile("assets_new/Idle_Up-Sheet.png");
    tIdleSide.loadFromFile("assets_new/Idle_Side-Sheet.png");

    tWalkDown.loadFromFile("assets_new/Walk_Down-Sheet.png");
    tWalkUp.loadFromFile("assets_new/Walk_Up-Sheet.png");
    tWalkSide.loadFromFile("assets_new/Walk_Side-Sheet.png");

    tSliceDown.loadFromFile("assets_new/Slice_Down-Sheet.png");
    tSliceUp.loadFromFile("assets_new/Slice_Up-Sheet.png");
    tSliceSide.loadFromFile("assets_new/Slice_Side-Sheet.png");

    tCollectDown.loadFromFile("assets_new/Collect_Down-Sheet.png");
    tCollectUp.loadFromFile("assets_new/Collect_Up-Sheet.png");
    tCollectSide.loadFromFile("assets_new/Collect_Side-Sheet.png");

    tCrushDown.loadFromFile("assets_new/Crush_Down-Sheet.png");
    tCrushUp.loadFromFile("assets_new/Crush_Up-Sheet.png");
    tCrushSide.loadFromFile("assets_new/Crush_Side-Sheet.png");

    sprite.setTexture(tIdleDown);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setOrigin(32, 48); // Bottom-ish center
    sprite.setScale(3.0f, 3.0f);
    sprite.setPosition(400, 300); // Keep initial position

    // Starting inventory
    inv.wood = 0;
    inv.food = 10;
    inv.water = 10;
    inv.stone = 0;
}

void Player::triggerAttack()
{
    if (currentState != ATTACKING && currentState != EATING && currentState != DRINKING) {
        currentState = ATTACKING;
        animFrame = 0;
        animClock.restart();
    }
}

void Player::triggerEat()
{
    if (currentState != ATTACKING && currentState != EATING && currentState != DRINKING) {
        currentState = EATING;
        animFrame = 0;
        animClock.restart();
    }
}

void Player::triggerDrink()
{
    if (currentState == IDLE || currentState == WALKING) {
        currentState = DRINKING; animFrame = 0; animClock.restart();
    }
}

void Player::triggerHarvest()
{
    if (currentState == IDLE || currentState == WALKING) {
        currentState = HARVESTING; animFrame = 0; animClock.restart();
    }
}

void Player::triggerCrush()
{
    if (currentState == IDLE || currentState == WALKING) {
        currentState = CRUSHING; animFrame = 0; animClock.restart();
    }
}

void Player::handleInput()
{
    if (currentState != IDLE && currentState != WALKING) return; 

    sf::Vector2f move(0, 0);
    bool moving = false;
// ... (W, S, A, D logic omitted for brevity here, but included in the tool call)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { move.y -= speed; currentDir = UP; moving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { move.y += speed; currentDir = DOWN; moving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { move.x -= speed; currentDir = SIDE; moving = true; facingLeft = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { move.x += speed; currentDir = SIDE; moving = true; facingLeft = false; }

    if (moving)
    {
        sprite.move(move);
        currentState = WALKING;
    }
    else
    {
        currentState = IDLE;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && currentState != ATTACKING)
    {
        triggerAttack();
    }
}

void Player::update()
{
    handleInput();

    // Dynamic Animation Timing
    int frameWidth = 64;
    int frameHeight = 64;
    int maxFrames = 4;
    int frameDuration = 100; // Default

    if (currentState == IDLE) frameDuration = 150;
    else if (currentState == WALKING) frameDuration = 100;
    else if (currentState == ATTACKING) frameDuration = 60;
    else if (currentState == HARVESTING || currentState == CRUSHING) frameDuration = 70;
    else if (currentState == EATING || currentState == DRINKING) frameDuration = 120;

    if (animClock.getElapsedTime().asMilliseconds() > frameDuration)
    {
        animFrame++;
        animClock.restart();
    }

    sprite.setColor(sf::Color::White); 
    
    // Smooth Scale/Breathing Logic
    static sf::Clock breathClock;
    float time = breathClock.getElapsedTime().asSeconds();
    float scaleOffset = 0.05f * std::sin(time * 3.0f); // Breathing pulse
    float baseScale = 3.0f;

    if (currentState == ATTACKING)
    {
        maxFrames = 6;
        if (animFrame >= maxFrames) { currentState = IDLE; animFrame = 0; }
        if (currentDir == DOWN) sprite.setTexture(tSliceDown);
        else if (currentDir == UP) sprite.setTexture(tSliceUp);
        else sprite.setTexture(tSliceSide);
        baseScale = 3.1f; 
    }
    else if (currentState == HARVESTING)
    {
        maxFrames = 8;
        if (animFrame >= maxFrames) { currentState = IDLE; animFrame = 0; }
        if (currentDir == DOWN) sprite.setTexture(tCollectDown);
        else if (currentDir == UP) sprite.setTexture(tCollectUp);
        else sprite.setTexture(tCollectSide);
    }
    else if (currentState == CRUSHING)
    {
        maxFrames = 8;
        if (animFrame >= maxFrames) { currentState = IDLE; animFrame = 0; }
        if (currentDir == DOWN) sprite.setTexture(tCrushDown);
        else if (currentDir == UP) sprite.setTexture(tCrushUp);
        else sprite.setTexture(tCrushSide);
    }
    else if (currentState == EATING)
    {
        maxFrames = 4;
        sprite.setColor(sf::Color(255, 200, 200)); 
        if (animFrame >= maxFrames) { currentState = IDLE; animFrame = 0; }
        sprite.setTexture(tIdleDown); 
    }
    else if (currentState == DRINKING)
    {
        maxFrames = 8;
        sprite.setColor(sf::Color(200, 200, 255)); 
        if (animFrame >= maxFrames) { currentState = IDLE; animFrame = 0; }
        sprite.setTexture(tIdleDown); 
    }
    else if (currentState == WALKING)
    {
        maxFrames = 6;
        animFrame %= maxFrames;
        if (currentDir == DOWN) sprite.setTexture(tWalkDown);
        else if (currentDir == UP) sprite.setTexture(tWalkUp);
        else sprite.setTexture(tWalkSide);
        baseScale += 0.15f * std::abs(std::sin(time * 12.0f)); 
    }
    else // IDLE
    {
        maxFrames = 4;
        animFrame %= maxFrames;
        if (currentDir == DOWN) sprite.setTexture(tIdleDown);
        else if (currentDir == UP) sprite.setTexture(tIdleUp);
        else sprite.setTexture(tIdleSide);
        baseScale += scaleOffset;
    }

    if (currentDir == SIDE)
        sprite.setScale(facingLeft ? -baseScale : baseScale, baseScale);
    else
        sprite.setScale(baseScale, baseScale);

    sprite.setTextureRect(sf::IntRect(animFrame * frameWidth, 0, frameWidth, frameHeight));
}

int Player::getAttackPower()
{
    if (currentState == ATTACKING && animFrame == 3) return attackPower;
    if (currentState == HARVESTING && animFrame == 4) return attackPower / 2;
    if (currentState == CRUSHING && animFrame == 4) return attackPower / 2;
    return 0;
}
