#pragma once
#include "Enemy.h"
#include <string>
using namespace std;

class EnemyFactory {
public:
    static Enemy* create(const string& type, double x, double y); //Declares the main public function
    //static means you can call it without creating EnemyFactory object, you just call it directly as EnemyFactory::create(...)

private:
    static Enemy* createBottom(double x, double y); //Declares the five individual creation functions
    static Enemy* createFooga(double x, double y); //create() can call them, but not outside code
    static Enemy* createTornado(double x, double y); //static for the same reason as above
    static Enemy* createMogera(double x, double y); // made private bcuz outside code should only ever use the one public create()
    static Enemy* createGamakichi(double x, double y);
};