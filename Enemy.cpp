#include "Enemy.h"
#include <cmath>
#include <iostream>

// -----------------------------------------------------------------------
// Sprite sheet specs for each enemy type
// -----------------------------------------------------------------------
// ORC      idle: 32x32 x4 frames  | run: 64x64 x6  | death: 64x64 x4
// SKELETON idle: 64x64 x4 frames  | run: 64x64 x6  | death: 64x64 x8
// KNIGHT   idle: 32x32 x4 frames  | run: 64x32 x6  | death: 32x32 x9
// ROGUE    idle: 32x32 x4 frames  | run: 64x32 x6  | death: 32x32 x12
// WIZZARD  idle: 32x32 x4 frames  | run: 64x32 x6  | death: 32x32 x12
// -----------------------------------------------------------------------

Enemy::Enemy(float x, float y, EType type) : isDeadFinished(false), mobType(type)
{
    switch (mobType)
    {
    case SKELETON:
        health = 60; maxHealth = 60; speed = 2.0f;
        tIdle.loadFromFile("assets_new/skel_idle.png");
        tRun.loadFromFile("assets_new/skel_run.png");
        tDeath.loadFromFile("assets_new/skel_death.png");
        break;

    case KNIGHT:
        health = 120; maxHealth = 120; speed = 2.4f;
        tIdle.loadFromFile("assets_new/knight_idle.png");
        tRun.loadFromFile("assets_new/knight_run.png");
        tDeath.loadFromFile("assets_new/knight_death.png");
        break;

    case ROGUE:
        health = 70; maxHealth = 70; speed = 3.8f;   // fast & fragile
        tIdle.loadFromFile("assets_new/rogue_idle.png");
        tRun.loadFromFile("assets_new/rogue_run.png");
        tDeath.loadFromFile("assets_new/rogue_death.png");
        break;

    case WIZZARD:
        health = 50; maxHealth = 50; speed = 1.8f;   // slow but glass cannon
        tIdle.loadFromFile("assets_new/wizzard_idle.png");
        tRun.loadFromFile("assets_new/wizzard_run.png");
        tDeath.loadFromFile("assets_new/wizzard_death.png");
        break;

    default: // ORC
        health = 80; maxHealth = 80; speed = 2.8f;
        tIdle.loadFromFile("assets_new/orc_idle.png");
        tRun.loadFromFile("assets_new/orc_run.png");
        tDeath.loadFromFile("assets_new/enemy_run.png");
        break;
    }

    sprite.setTexture(tIdle);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setOrigin(16, 16);
    sprite.setScale(3.5f, 3.5f);
    sprite.setPosition(x, y);
}

// --------------- Helpers to get frame info per type ----------------

static int idleFrameSize(Enemy::EType t)  { return (t == Enemy::SKELETON) ? 64 : 32; }
static int idleFrameCount(Enemy::EType t) { return 4; }
static int runFrameW(Enemy::EType t)      { return 64; }
static int runFrameH(Enemy::EType t)
{
    if (t == Enemy::KNIGHT || t == Enemy::ROGUE || t == Enemy::WIZZARD) return 32;
    return 64; // ORC / SKELETON
}
static int runFrameCount(Enemy::EType)   { return 6; }
static int deathFrameW(Enemy::EType t)
{
    if (t == Enemy::KNIGHT)  return 32;
    if (t == Enemy::ROGUE)   return 32;
    if (t == Enemy::WIZZARD) return 32;
    return 64; // ORC / SKELETON
}
static int deathFrameCount(Enemy::EType t)
{
    if (t == Enemy::SKELETON) return 8;
    if (t == Enemy::KNIGHT)   return 9;
    if (t == Enemy::ROGUE)    return 12;
    if (t == Enemy::WIZZARD)  return 12;
    return 4; // ORC
}
// -------------------------------------------------------------------

void Enemy::update()
{
    if (health <= 0)
    {
        if (currentState != DEAD) {
            currentState = DEAD; animFrame = 0; animClock.restart();
            sprite.setTexture(tDeath);
            sprite.setColor(sf::Color::White);
            int dw = deathFrameW(mobType);
            sprite.setTextureRect({0, 0, dw, dw});
            sprite.setOrigin(dw / 2.f, dw / 2.f);
        }

        if (animClock.getElapsedTime().asMilliseconds() > 100) {
            animClock.restart();
            int maxDF = deathFrameCount(mobType);
            if (animFrame >= maxDF) {
                isDeadFinished = true;
            } else {
                int dw = deathFrameW(mobType);
                sprite.setTextureRect({animFrame * dw, 0, dw, dw});
                animFrame++;
            }
        }
        return;
    }

    sf::Vector2f enemyPos = sprite.getPosition();
    float dx = targetPos.x - enemyPos.x;
    float dy = targetPos.y - enemyPos.y;
    float distance = std::sqrt(dx*dx + dy*dy);

    if (distance > 5.f && distance < 900.f)
    {
        sprite.move(dx / distance * speed, dy / distance * speed);
        currentState = RUNNING;
        float sc = 3.5f;
        sprite.setScale(dx < 0 ? -sc : sc, sc);
    }
    else
    {
        currentState = IDLE;
        sprite.setScale(3.5f, 3.5f);
    }

    // Animation
    int frameDuration = (currentState == RUNNING) ? 80 : 150;
    if (animClock.getElapsedTime().asMilliseconds() > frameDuration)
    {
        animFrame++;
        animClock.restart();

        if (currentState == RUNNING)
        {
            int fw = runFrameW(mobType);
            int fh = runFrameH(mobType);
            animFrame %= runFrameCount(mobType);
            sprite.setTexture(tRun);
            sprite.setTextureRect(sf::IntRect(animFrame * fw, 0, fw, fh));
            sprite.setOrigin(fw / 2.f, fh / 2.f);
        }
        else
        {
            int fs = idleFrameSize(mobType);
            animFrame %= idleFrameCount(mobType);
            sprite.setTexture(tIdle);
            sprite.setTextureRect(sf::IntRect(animFrame * fs, 0, fs, fs));
            sprite.setOrigin(fs / 2.f, fs / 2.f);
        }
    }
}

void Enemy::draw(sf::RenderWindow& window)
{
    if (health <= 0) return;
    window.draw(sprite);

    // Colour-coded health bar per type
    sf::Color barColour;
    switch (mobType) {
        case KNIGHT:  barColour = {80,  160, 255}; break; // Blue
        case ROGUE:   barColour = {180, 50,  255}; break; // Purple
        case WIZZARD: barColour = {50,  220, 220}; break; // Cyan
        case SKELETON:barColour = {220, 220, 200}; break; // White-ish
        default:      barColour = {255, 80,  50 }; break; // Red (orc)
    }

    float barWidth = 50.f;
    sf::RectangleShape bg({barWidth, 5});
    bg.setFillColor(sf::Color(30, 30, 30, 180));
    bg.setPosition(sprite.getPosition().x - barWidth/2.f, sprite.getPosition().y - 55);

    sf::RectangleShape fg({barWidth * ((float)health / maxHealth), 5});
    fg.setFillColor(barColour);
    fg.setPosition(sprite.getPosition().x - barWidth/2.f, sprite.getPosition().y - 55);

    window.draw(bg);
    window.draw(fg);
}

void Enemy::takeDamage(int dmg)  { health -= dmg; }
bool Enemy::isDead()             { return health <= 0; }
void Enemy::setTarget(sf::Vector2f target) { targetPos = target; }
sf::Vector2f Enemy::getPosition() const    { return sprite.getPosition(); }
