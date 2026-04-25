#pragma once
#include "Enemy.h"
#include <string>
using namespace std;

class EnemyFactory {
public:
    static Enemy* create(const string& type, double x, double y);

private:
    static Enemy* createBottom(double x, double y);
    static Enemy* createFooga(double x, double y);
    static Enemy* createTornado(double x, double y);
    static Enemy* createMogera(double x, double y);
    static Enemy* createGamakichi(double x, double y);
};