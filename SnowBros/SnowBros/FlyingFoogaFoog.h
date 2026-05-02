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

    // Shadow Botom's HITS_TO_ENCASE — Foog needs 6 hits instead of 4
    static constexpr int HITS_TO_ENCASE = 6;

    void pickRandomFlightDirection();

    // ── Sprite sheet rects ────────────────────────────────
    // Row 1: Flying / flapping (3 frames)
    sf::IntRect fly_frame1{ { 44,  32}, {110, 156} };
    sf::IntRect fly_frame2{ {208,  18}, {176, 172} };
    sf::IntRect fly_frame3{ {434,  26}, {138, 154} };

    // Row 2: Gliding / hovering (3 frames)
    sf::IntRect glide_frame1{ { 44, 240}, {134, 138} };
    sf::IntRect glide_frame2{ {234, 260}, {137, 116} };
    sf::IntRect glide_frame3{ {411, 221}, {152, 151} };

    // Row 3: Angry / charging (2 frames)
    sf::IntRect angry_frame1{ { 25, 427}, {158, 129} };
    sf::IntRect angry_frame2{ {218, 432}, {155, 123} };

    // Row 4: Getting topped / encased (3 frames)
    sf::IntRect topped_fooga_frame1{ { 24, 611}, {161, 140} };
    sf::IntRect topped_fooga_frame2{ {222, 638}, {158, 114} };
    sf::IntRect topped_fooga_frame3{ {415, 664}, {145,  86} };

    // Row 5: Frozen / fully encased (1 frame)
    sf::IntRect frozen_fooga{ {49, 813}, {134, 150} };

public:
    FlyingFoogaFoog(double startX, double startY, float scrW = 800.f, float scrH = 600.f);
    void loadTexture(const std::string& path);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    // Override receiveSnowballHit so it uses our HITS_TO_ENCASE = 6
    bool receiveSnowballHit();
};