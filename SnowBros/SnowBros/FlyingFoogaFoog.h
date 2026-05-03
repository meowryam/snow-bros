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
    static constexpr int HITS_TO_ENCASE = 6;

    void pickRandomFlightDirection();

    IntRect fly_frame1{ { 44,  32}, {110, 156} };
    IntRect fly_frame2{ {208,  18}, {176, 172} };
    IntRect fly_frame3{ {434,  26}, {138, 154} };
    IntRect glide_frame1{ { 44, 240}, {134, 138} };
    IntRect glide_frame2{ {234, 260}, {137, 116} };
    IntRect glide_frame3{ {411, 221}, {152, 151} };
    IntRect angry_frame1{ { 25, 427}, {158, 129} };
    IntRect angry_frame2{ {218, 432}, {155, 123} };
    IntRect topped_fooga_frame1{ { 24, 611}, {161, 140} };
    IntRect topped_fooga_frame2{ {222, 638}, {158, 114} };
    IntRect topped_fooga_frame3{ {415, 664}, {145,  86} };
    IntRect frozen_fooga{ {49, 813}, {134, 150} };

public:
    FlyingFoogaFoog(double startX, double startY, float scrW = 800.f, float scrH = 600.f);
    void loadTexture(const std::string& path);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    bool receiveSnowballHit();
};