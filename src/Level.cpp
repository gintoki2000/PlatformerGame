#include "Level.h"
#include "Box2D/Box2D.h"
#include "Contances.h"

Level* Level::create(const std::string& filename)
{
    Level* ret = new Level;
    if (ret && ret->initialize(filename))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Level::initialize(const std::string&)
{
    m_world = new b2World(Contances::GRAVITY);
    return true;
}

Level::Level() : m_world(nullptr) {}

Level::~Level() { delete m_world; }
