

#pragma once
#include "Platform.h"

class LevelLayout {
public:
    static const int MAX_PLATFORMS = 20;

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
         
            float ph = 24.f;
            platforms[count++] = Platform(20, H - 150.f, 300, ph, sf::Color(60, 100, 160));  
            platforms[count++] = Platform(60, H - 290.f, 260, ph, sf::Color(60, 100, 160));  
            platforms[count++] = Platform(20, H - 430.f, 300, ph, sf::Color(60, 100, 160));  
            return count;
        }

       
        float p1y = H - 160.f;
        float p2y = H - 290.f;
        float p3y = H - 420.f;

 // Row 1 
        platforms[count++] = Platform(20, p1y, 200, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(300, p1y, 160, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(540, p1y, 220, ph, sf::Color(60, 100, 160));

 // Row 2 
        platforms[count++] = Platform(0, p2y, 160, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(240, p2y, 300, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(620, p2y, 180, ph, sf::Color(60, 100, 160));
 // Row 3
        platforms[count++] = Platform(60, p3y, 180, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(320, p3y, 140, ph, sf::Color(60, 100, 160));
        platforms[count++] = Platform(540, p3y, 200, ph, sf::Color(60, 100, 160));

        return count;
    }
};