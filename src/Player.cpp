#include "Player.h"
#include "Animation.h"
#include "Box2D/Box2D.h"
#include "Constances.h"
#include "Game.h"
#include "Level.h"
#include "NTTextureRegion.h"
#include "SDL_error.h"
#include "SDL_image.h"
#include "SDL_render.h"
Player::Player() :
    m_isOnGround(false), m_animationState(ANIMATION_IDLE_1), m_body(nullptr),
    m_level(nullptr)
{
}

Player::~Player()
{
    m_level->getWorld()->DestroyBody(m_body);
    SDL_DestroyTexture(m_texture);
}

static std::vector<NTTextureRegion>
create_sprite_sheet(SDL_Texture* texture, int startX, int startY, int rows,
                    int cols, int spriteWidth, int spriteHeight)
{
    std::vector<NTTextureRegion> spriteSheet(rows * cols);
    SDL_Rect rect{startX, startY, spriteWidth, spriteHeight};
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            rect.x = startX + j * spriteWidth;
            rect.y = startY + i * spriteHeight;
            spriteSheet[i * rows + j] = NTTextureRegion(texture, rect);
        }
    return spriteSheet;
}

Player* Player::create(Level* level)
{
    Player* ret = new Player;
    if (ret->initialize(level))
        return ret;
    delete ret;
    return nullptr;
}

bool Player::initialize(Level* level)
{

    SDL_Surface* surface = IMG_Load("asserts/player.png");
    if (surface == nullptr)
    {
        SDL_Log("Could not load texture: %s", SDL_GetError());
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(Game::getInstance()->getRenderer(),
                                             surface);

    m_animations[ANIMATION_IDLE_1] = Animation<NTTextureRegion>(
        create_sprite_sheet(m_texture, 0, 0, 1, 4, SPRITE_WIDTH, SPRITE_HEIGHT),
        1.f, ANIMATION_TYPE_NORMAL);
    m_animations[ANIMATION_IDLE_2] = Animation<NTTextureRegion>(
        create_sprite_sheet(m_texture, 100, 185, 1, 4, SPRITE_WIDTH,
                            SPRITE_HEIGHT),
        1.f, ANIMATION_TYPE_LOOP);

    m_level = level;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    bodyDef.position.x = Game::WIDTH / 2.f / Constances::PPM;
    bodyDef.position.y = Game::HEIGHT / 2.f / Constances::PPM;

    m_body = level->getWorld()->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(WIDTH / 2.f / Constances::PPM,
                   HEIGHT / 2.f / Constances::PPM);
    b2FixtureDef fixtureDef;
    fixtureDef.userData = this;
    fixtureDef.shape = &shape;

    m_body->CreateFixture(&fixtureDef);

    return true;
}

void Player::update(float dt) { m_animationTimeState += dt; }

void Player::draw(SDL_Renderer* renderer, const NTRect& viewPort)
{
    const NTTextureRegion& currentFrame =
        m_animations[m_animationState].getCurrentFrame(m_animationTimeState);
    currentFrame.draw(renderer,
        m_body->GetPosition().x * Constances::PPM - SPRITE_WIDTH / 2,
        m_body->GetPosition().y * Constances::PPM - SPRITE_HEIGHT);
}
