#include "MainState.h"
#include "Level.h"

MainState* MainState::create()
{
    MainState* ret = new MainState;
    if (ret->initialize())
        return ret;
    delete ret;
    return nullptr;
}

MainState::~MainState() { delete m_level; }

MainState::MainState() : m_level(nullptr) {}

bool MainState::initialize()
{
    if ((m_level = Level::create("")) == nullptr)
    {
        return false;
    }
    return true;
}

void MainState::update(float dt) { m_level->update(dt); }

void MainState::draw(SDL_Renderer* renderer) { m_level->draw(renderer); }
