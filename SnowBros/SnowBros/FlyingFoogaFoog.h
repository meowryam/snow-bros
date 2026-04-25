#pragma once
#include "Botom.h"


class FlyingFoogaFoog : public Botom {

protected:
    bool inFlight;
    float groundTimer;
    static constexpr float GROUND_DURATION = 3.0f;  
    float flightTimer;
    static constexpr float FLIGHT_DURATION = 4.0f; 
    float flightDirTimer;
    static constexpr float FLIGHT_DIR_INTERVAL = 1.0f;  // seconds between direction changes
    static constexpr float FLIGHT_SPEED = 160.f;
    void pickRandomFlightDirection();

public:
    FlyingFoogaFoog(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override; //blue
};