#pragma once
#include<string>
#include "Enemy.h"
using namespace std;


// Forward declarations of all concrete enemy types
// (actual headers included in EnemyFactory.cpp to avoid circular deps)
class Botom;
class FlyingFoogaFoog;
class Tornado;
class Mogera;
class Gamakichi;

// EnemyFactory — Design Pattern: Factory Method
// Creates enemy objects by type string without the caller
// needing to know which class to instantiate.
// Returns a raw Enemy* (caller owns the pointer — must delete it)

class EnemyFactory {
public:

    // Valid type strings: "botom", "fooga", "tornado", "mogera", "gamakichi"
    static Enemy* create(const string& type, double x, double y) {
        if (type == "botom")     return createBottom(x, y);
        else if (type == "fooga")     return createFooga(x, y);
        else if (type == "tornado")   return createTornado(x, y);
        else if (type == "mogera")    return createMogera(x, y);
        else if (type == "gamakichi") return createGamakichi(x, y);
        else return nullptr; // unknown type
    }

private:
    static Enemy* createBottom(double x, double y);
    static Enemy* createFooga(double x, double y);
    static Enemy* createTornado(double x, double y);
    static Enemy* createMogera(double x, double y);
    static Enemy* createGamakichi(double x, double y);
};