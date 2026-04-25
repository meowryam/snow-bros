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

        // Every level has a floor
        platforms[count++] = Platform(0, H - 24, W, 24, sf::Color(60, 100, 160));

        if (levelNo == 5 || levelNo == 10) {
            // Boss levels — open floor, just the ground
            platforms[count++] = Platform(0, H - 24, W, 24, sf::Color(80, 20, 20));
            return count;
        }

        // Standard levels — 3 platform rows
        float p1y = H - 160.f;  // row 1
        float p2y = H - 300.f;  // row 2
        float p3y = H - 440.f;  // row 3

        // Row 1
        platforms[count++] = Platform(50, p1y, 220, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(320, p1y, 160, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(530, p1y, 220, 18, sf::Color(60, 100, 160));

        // Row 2
        platforms[count++] = Platform(0, p2y, 180, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(230, p2y, 340, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(620, p2y, 180, 18, sf::Color(60, 100, 160));

        // Row 3
        platforms[count++] = Platform(80, p3y, 200, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(340, p3y, 120, 18, sf::Color(60, 100, 160));
        platforms[count++] = Platform(520, p3y, 200, 18, sf::Color(60, 100, 160));

        return count;
    }
};