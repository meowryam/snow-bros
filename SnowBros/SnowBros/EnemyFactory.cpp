#include "EnemyFactory.h"
#include "Botom.h"
#include "FlyingFoogaFoog.h"
#include "Tornado.h"
#include "Mogera.h"
#include "Gamakichi.h"

Enemy* EnemyFactory::create(const string& type, double x, double y) {
    if (type == "botom")     return createBottom(x, y);
    else if (type == "fooga")     return createFooga(x, y);
    else if (type == "tornado")   return createTornado(x, y);
    else if (type == "mogera")    return createMogera(x, y);
    else if (type == "gamakichi") return createGamakichi(x, y);
    else return nullptr;
}

Enemy* EnemyFactory::createBottom(double x, double y) { return new Botom(x, y); }
Enemy* EnemyFactory::createFooga(double x, double y) { return new FlyingFoogaFoog(x, y); }
Enemy* EnemyFactory::createTornado(double x, double y) { return new Tornado(x, y); }
Enemy* EnemyFactory::createMogera(double x, double y) { return new Mogera(x, y); }
Enemy* EnemyFactory::createGamakichi(double x, double y) { return new Gamakichi(x, y); }