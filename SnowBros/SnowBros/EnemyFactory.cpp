#include "EnemyFactory.h"
#include "Botom.h"
#include "FlyingFoogaFoog.h"
#include "Tornado.h"
#include "Mogera.h"
#include "Gamakichi.h"


Enemy* EnemyFactory::create(const string& type, double x, double y)
// A function belonging to EnemyFactory that takes an enemy type as a string and x/y coordinates. It returns a pointer to an Enemy object (since Enemy is abstract, the actual object will be one of its subclasses).
{
    if (type == "botom")//Checks the string and calls the matching creation function. Each branch handles one specific enemy type.
        return createBottom(x, y);
    else if (type == "fooga")   
        return createFooga(x, y);
    else if (type == "tornado") 
        return createTornado(x, y);
    else if (type == "mogera")   
        return createMogera(x, y);
    else if (type == "gamakichi")
        return createGamakichi(x, y);
    else return nullptr; //If the string doesn't match any known enemy type, it returns nullptr (essentially "nothing") instead of crashing.
}

//Each of these creates the specific enemy object on the heap using new, passing the x/y
//position, and returns it as an Enemy* pointer. Even though the actual object is a Botom or
// Tornado etc., it's returned as the parent type Enemy* — this works because of inheritance.
Enemy* EnemyFactory::createBottom(double x, double y)
{ return new Botom(x, y); }
Enemy* EnemyFactory::createFooga(double x, double y) 
{ return new FlyingFoogaFoog(x, y); }
Enemy* EnemyFactory::createTornado(double x, double y) 
{ return new Tornado(x, y); }
Enemy* EnemyFactory::createMogera(double x, double y) 
{ return new Mogera(x, y); }
Enemy* EnemyFactory::createGamakichi(double x, double y)
{ return new Gamakichi(x, y); }

// inshort: This is the Factory design pattern. Instead of your game code doing new Tornado(x, y) directly everywhere
//it just calls EnemyFactory::create("tornado", x, y). This keeps enemy creation in one place
// so if you ever add a new enemy you only change the factory, not every place in your code that spawns enemies.