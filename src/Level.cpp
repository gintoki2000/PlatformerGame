#include "Level.h"

Level::Level() { m_assertManager = new AssertManager(); }

Level::~Level() { delete m_assertManager; }

bool Level::init(const std::string& filename) { return true; }

void Level::render(float deltaTime) {}

