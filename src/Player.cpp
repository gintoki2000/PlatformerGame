#include "Player.h"
#include "Level.h"
#include "Box2D/Box2D.h"
#include "Constances.h"
Player::Player():
		m_isOnGround(false),
		m_animationState(ANIMATION_IDLE_1),
		m_body(nullptr),
		m_level(nullptr)
{
	
}

Player::~Player()
{
}

bool Player::initialize(Level* level)
{
	m_level = level;
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.userData = this;

	m_body = level->getWorld()->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT / 2.f * Constances::PPM);
	b2FixtureDef fixtureDef;
	fixtureDef.userData = this;
	fixtureDef.shape = &shape;
	
	m_body->CreateFixture(&fixtureDef);

	return true;
}
