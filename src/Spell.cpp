#include "Spell.h"
#include "Constances.h"
#include "Enums.h"

Spell::Spell(Level* level) :
    GameObject(GAME_OBJECT_TYPE_SPELL, level), m_target(0), m_damage(0),
    m_width(Constances::TILE_SIZE), m_height(Constances::TILE_SIZE),
    m_direction(DIRECTION_LEFT), m_animation(nullptr), m_body(nullptr),
    m_elpasedTime(0.f)
{
}
