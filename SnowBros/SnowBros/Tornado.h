#pragma once
#include "FlyingFoogaFoog.h"

struct Knife {

    double x, y;           
    double xspeed, yspeed; 
    bool   alive;          
    static constexpr float knifehitboxwidth = 16.f;
    static constexpr float knifehitboxheight = 8.f;

    Knife() : x(0), y(0), xspeed(0), yspeed(0), alive(false) {}
    void update(double deltaTime) {
        if (!alive) return;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;
    }

    FloatRect getHitbox() const {
        return FloatRect(
            Vector2f(static_cast<float>(x), static_cast<float>(y)),
            Vector2f(knifehitboxwidth, knifehitboxheight)
        );
    }
    void draw(sf::RenderWindow& window) const {
        if (!alive) return;
        RectangleShape rect(Vector2f(knifehitboxwidth, knifehitboxheight));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(Color(255, 220, 50));  
        window.draw(rect);
    }
};



class Tornado : public FlyingFoogaFoog {
public:
    static const int MAX_KNIVES = 6;  
    Knife knives[MAX_KNIVES];
private:
    double playerX, playerY;
    float knifeTimer;
    static constexpr float KNIFE_INTERVAL = 2.5f;  
    static constexpr float KNIFE_SPEED = 350.f;  
    float currentFlightSpeed;
    static constexpr float MIN_FLIGHT_SPEED = 60.f;   
    static constexpr float MAX_FLIGHT_SPEED = 320.f;  
    void fireKnife();
    void randomizeFlightSpeed();


private:
    // ── Sprite rects ─────────────────────────────────────
    // Idle
    sf::IntRect t_idle{ { 15,  38}, {108, 122} };

    // Running (3 frames)
    sf::IntRect t_run1{ {147,  40}, {121, 119} };
    sf::IntRect t_run2{ {286,  40}, {119, 118} };
    sf::IntRect t_run3{ {420,  35}, {129, 123} };

    // Jumping
    sf::IntRect t_jump1{ { 31, 202}, { 97, 138} };
    // Trapped (encased in snow)
    sf::IntRect t_trapped1{ {158, 210}, {109, 120} };
    sf::IntRect t_trapped2{ {293, 222}, {112, 105} };

    // Freeing itself (breaking out of snow, 3 frames)
    sf::IntRect t_free1{ { 16, 377}, {118, 117} };
    sf::IntRect t_free2{ {159, 377}, {113, 117} };
    sf::IntRect t_free3{ {291, 376}, { 97, 118} };

    // Falling
    sf::IntRect t_falling{ { 10, 536}, {129, 137} };
    // Landing
    sf::IntRect t_landing{ {154, 548}, {109, 114} };

    // Running / turning (2 frames)
    sf::IntRect t_turning1{ { 20, 710}, {112, 119} };
    sf::IntRect t_turning2{ {152, 714}, {118, 116} };

    // Furning / puffing up (2 frames)
    sf::IntRect t_puff1{ { 27, 881}, {112, 116} };
    sf::IntRect t_puff2{ {140, 884}, {131, 112} };

    // Throwing knife (2 frames)
    sf::IntRect t_throw1{ { 24, 1046}, { 93, 120} };
    sf::IntRect t_throw2{ {143, 1047}, {136, 120} };

    // Knives (4 sprites)
    sf::IntRect t_knife_red{ {305, 1043}, { 29,  37} };
    sf::IntRect t_knife_yellow{ {320,  883}, { 29,  37} };
    sf::IntRect t_knife_blue{ {352,  890}, { 66,  30} };
    sf::IntRect t_knife_pink{ {425,  892}, { 66,  26} };

    // Spinning transformation (8 frames — becomes tornado)
    sf::IntRect t_spin1{ {  5, 1208}, {138, 129} };
    sf::IntRect t_spin2{ {147, 1208}, {126, 138} };
    sf::IntRect t_spin3{ {302, 1215}, { 91, 124} };
    sf::IntRect t_spin4{ {413, 1217}, {144, 121} };
    sf::IntRect t_spin5{ {578, 1218}, { 98, 121} };
    sf::IntRect t_spin6{ {692, 1216}, {143, 122} };
    sf::IntRect t_spin7{ {844, 1222}, {115, 111} };
    sf::IntRect t_spin8{ {985, 1199}, {117, 135} };
    sf::IntRect t_spin9{ {1115,1203}, {129, 132} };

    // Dying (8 frames)
    sf::IntRect t_die1{ {417, 1046}, {139, 120} };
    sf::IntRect t_die2{ {557, 1047}, { 87, 128} };
    sf::IntRect t_die3{ {719, 1063}, { 88, 115} };
    sf::IntRect t_die4{ {888, 1050}, { 84, 122} };
    sf::IntRect t_die5{ {995, 1050}, { 90, 102} };

    // Tornado rings (2 frames — portal/ring effect)
    sf::IntRect t_ring1{ {619, 873}, {117, 120} };
    sf::IntRect t_ring2{ {751, 873}, {121, 121} };

    // add to private:
    sf::Texture tornadoTexture;
    std::optional<sf::Sprite> tornadoSprite;
    bool tornadoTextureLoaded = false;
public:

    Tornado(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f);
    void setPlayerPos(double px, double py);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void loadTexture(const std::string& path);
 
};