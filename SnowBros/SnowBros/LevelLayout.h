/*
#pragma once
#include "Platform.h"

// Hardcoded platform layouts for each level
// Returns number of platforms written into the array
class LevelLayout {
public:
    static const int MAX_PLATFORMS = 20;

    // Standard 800x600 layout — floor + 3 rows of platforms
    static int getLayout(int levelNo, Platform platforms[], float W = 800.f, float H = 600.f) {
        int count = 0;
        float ph =24.f;

        platforms[count++] = Platform(0, H - 40, W, 40, sf::Color(60, 100, 160));

        if (levelNo == 5 || levelNo == 10) {
            return count;
        }

        // Bring rows DOWN — was too high
        float p1y = H - 140.f;  // row 1 near bottom
        float p2y = H - 260.f;  // row 2 mid
        float p3y = H - 380.f;  // row 3 upper

        // Row 1
        platforms[count++] = Platform(50, p1y, 220, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(320, p1y, 160, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(530, p1y, 220, ph, sf::Color(60, 100, 160));

        // Row 2
        platforms[count++] = Platform(0, p2y, 180, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(230, p2y, 340, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(620, p2y, 180, ph, sf::Color(60, 100, 160));

        // Row 3
        platforms[count++] = Platform(80, p3y, 200, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(340, p3y, 120, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(520, p3y, 200, ph, sf::Color(60, 100, 160));

        return count;
    }
        };

*/
  

#pragma once
#include "Platform.h"

// Hardcoded platform layouts for each level
// Returns number of platforms written into the array
class LevelLayout {
public:
    static const int MAX_PLATFORMS = 20;

    // Standard 800x600 layout ? floor + 3 rows of platforms
    static int getLayout(int levelNo, Platform platforms[], float W = 800.f, float H = 600.f) {
        int count = 0;
        float ph = 24.f;

        platforms[count++] = Platform(0, H - 40, W, 40, sf::Color(60, 100, 160));

        if (levelNo == 10) {
            float p1y = H - 160.f;
            float p2y = H - 300.f;
            platforms[count++] = Platform(30, p1y, 220, ph, sf::Color(60, 100, 160));
            platforms[count++] = Platform(550, p1y, 220, ph, sf::Color(60, 100, 160));
            platforms[count++] = Platform(260, p2y, 280, ph, sf::Color(60, 100, 160));
            return count;
        }

        if (levelNo == 5) {
            // Mogera boss arena - 3 wide platforms stacked on LEFT side only
            // Right side stays open so Mogera has room at bottom-right
            // Platform heights spread far apart so it feels dramatic not cramped
            float ph = 24.f;
            platforms[count++] = Platform(20, H - 150.f, 300, ph, sf::Color(60, 100, 160));  // low-left
            platforms[count++] = Platform(60, H - 290.f, 260, ph, sf::Color(60, 100, 160));  // mid-left
            platforms[count++] = Platform(20, H - 430.f, 300, ph, sf::Color(60, 100, 160));  // high-left
            return count;
        }

        // Bring rows DOWN ? was too high
        float p1y = H - 140.f;  // row 1 near bottom
        float p2y = H - 260.f;  // row 2 mid
        float p3y = H - 380.f;  // row 3 upper

        // Row 1
        platforms[count++] = Platform(50, p1y, 220, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(320, p1y, 160, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(530, p1y, 220, ph, sf::Color(60, 100, 160));

        // Row 2
        platforms[count++] = Platform(0, p2y, 180, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(230, p2y, 340, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(620, p2y, 180, ph, sf::Color(60, 100, 160));

        // Row 3
        platforms[count++] = Platform(80, p3y, 200, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(340, p3y, 120, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(520, p3y, 200, ph, sf::Color(60, 100, 160));

        return count;
    }
};