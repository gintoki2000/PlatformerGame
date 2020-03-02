#include "Level.h"
#include "box2d/box2d.h"
#include "Constances.h"
#include "Player.h"
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
    m_world = new b2World(Constances::GRAVITY);
	if((m_player = Player::create(this)) == nullptr)
	{
			return false;
	}	
    return true;
}

Level::Level() : m_world(nullptr) {}

Level::~Level() { delete m_world; }

void Level::update(float dt)
{
		m_player->update(dt);
		m_world->Step(dt, 2, 6);
}

void Level::draw(SDL_Renderer *renderer)
{
		m_player->draw(renderer, m_viewPort);
}
