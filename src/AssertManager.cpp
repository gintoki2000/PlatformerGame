#include "AssertManager.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

TextureManager::TextureManager()
{
    SDL_memset(m_textures, 0, NUM_TEXTURES * sizeof(SDL_Texture*));
}

TextureManager::~TextureManager()
{
    for (int i = 0; i < NUM_TEXTURES; ++i)
    {
        SDL_DestroyTexture(m_textures[i]);
    }
}

bool TextureManager::load(SDL_Renderer* renderer)
{
    struct Pair
    {
        const char* file;
        TextureID   id;
    };

    Pair pairs[] = {
        {"asserts/spritesheets/player.png", PLAYER},
        {"asserts/spritesheets/fire-ball.png", FIREBALL},
        {"asserts/spritesheets/fire-explosion.png", FIRE_EXPLOSION},
        {"asserts/spritesheets/throwing-axe.png", THROWING_AXE},
        {"asserts/spritesheets/ffire-bust.png", FIRE_BUST},
        {"asserts/spritesheets/grenade.png", GRENADE},
        {"asserts/spritesheets/blood-stain.png", BLOOD_STAIN},
        {"asserts/spritesheets/status-bar-2.png", STATUS_BAR},
        {"asserts/spritesheets/boar-warrior.png", BOAR_WARRIOR}};

    int num = sizeof (pairs) / sizeof (Pair);
    for (int i = 0; i < num; ++i)
    {
        m_textures[pairs[i].id] = IMG_LoadTexture(renderer, pairs[i].file);
        if (m_textures[pairs[i].id] == nullptr)
        {
            SDL_Log("Failed to load texture: %s", IMG_GetError());
            return false;
        }
    }
    return true;
}

SDL_Texture* TextureManager::getTexture(TextureID ID) { return m_textures[ID]; }

bool SoundManager::load()
{
    struct Pair
    {
        const char* file;
        SoundID     id;
    };

    Pair pairs[] = {
        {"asserts/sounds/fireball.wav", FIREBALL},
        {"asserts/sounds/impact.wav", IMPACT},
        {"asserts/sounds/bomb-explosion.wav", BOMB_EXPLOSION}};
    Mix_Chunk* sound;
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        if ((sound = Mix_LoadWAV(pairs[i].file)) == nullptr)
        {
            SDL_Log("Failed to load sound: %s", Mix_GetError());
            return false;
        }
        m_sounds[i] = sound;
    }
    return true;
}

Mix_Chunk* SoundManager::getSound(SoundID ID) { return m_sounds[ID]; }

SoundManager::SoundManager()
{
    SDL_memset(m_sounds, 0, NUM_SOUNDS * sizeof(Mix_Chunk*));
}

SoundManager::~SoundManager()
{
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        if (m_sounds[i] != nullptr)
        {
            Mix_FreeChunk(m_sounds[i]);
            m_sounds[i] = nullptr;
        }
    }
}
