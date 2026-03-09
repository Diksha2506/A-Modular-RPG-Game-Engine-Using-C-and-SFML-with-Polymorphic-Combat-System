#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Camera
{
private:
    sf::View view;
    float shakeTime    = 0.f;
    float shakeMag     = 0.f;
    float mapW         = 0.f;
    float mapH         = 0.f;

public:

    Camera(float width, float height)
    {
        view.setSize(width, height);
        view.setCenter(width / 2.f, height / 2.f);
    }

    void setMapBounds(float mw, float mh)
    {
        mapW = mw;
        mapH = mh;
    }

    void shake(float magnitude, float duration)
    {
        shakeMag  = magnitude;
        shakeTime = duration;
    }

    void followPlayer(Player& player)
    {
        sf::Vector2f target = player.getPosition();

        // --- Smooth lerp ---
        sf::Vector2f curr = view.getCenter();
        sf::Vector2f next = curr + (target - curr) * 0.12f;

        // --- Clamp so view stays inside the map ---
        float hw = view.getSize().x / 2.f;
        float hh = view.getSize().y / 2.f;
        next.x = std::max(hw, std::min(mapW - hw, next.x));
        next.y = std::max(hh, std::min(mapH - hh, next.y));

        view.setCenter(next);
    }

    void update(float dt)
    {
        if (shakeTime > 0.f)
        {
            shakeTime -= dt;
            float ox = (rand() % 100 / 100.f - 0.5f) * 2.f * shakeMag;
            float oy = (rand() % 100 / 100.f - 0.5f) * 2.f * shakeMag;
            sf::Vector2f c = view.getCenter();
            view.setCenter(c.x + ox, c.y + oy);
            if (shakeTime <= 0.f) shakeTime = 0.f;
        }
    }

    void apply(sf::RenderWindow& window)
    {
        window.setView(view);
    }

    sf::Vector2f getCenter() const { return view.getCenter(); }
};
