#include "CameraShaker.h"
#include "Camera.h"
#include "Utils.h"

CameraShaker::CameraShaker(): m_camera(nullptr){}

bool CameraShaker::init(Camera *camera)
{
    m_camera = camera;
    m_isShaking = false;
    return true;
}

CameraShaker *CameraShaker::create(Camera *camera)
{
    CameraShaker* ret = new CameraShaker;
    if (ret->init(camera))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

void CameraShaker::shake(float duration, int amplitude, int frequency)
{
    if (m_isShaking)
        return;
    m_isShaking      = true;
    m_duration       = duration;
    m_amplitude      = amplitude;
    m_waitTime       = 1.f / frequency;
    m_timer          = 0.f;
    m_waitTimer      = 0.f;
    m_dx             = 0.f;
    m_dy             = 0.f;
    m_initialOffsetX = m_camera->getCenter().x;
    m_initialOffsetY = m_camera->getCenter().y;
}

void CameraShaker::tick(float deltaTime)
{
    if (!m_isShaking)
        return;
    m_timer += deltaTime;
    m_waitTimer += deltaTime;

    if (m_timer >= m_duration)
    {
        m_isShaking = false;
        m_camera->setCenter({m_initialOffsetX, m_initialOffsetY});
        return;
    }

    if (m_waitTimer >= m_waitTime)
    {
        m_waitTimer -= m_waitTime;
        m_dx = randomRange(-m_amplitude, m_amplitude);
        m_dy = randomRange(-m_amplitude, m_amplitude);
    }

    Vec2 cameraOffset;
    cameraOffset.x += m_initialOffsetX + m_dx;
    cameraOffset.y += m_initialOffsetY + m_dy;

    m_camera->setCenter(cameraOffset);
}
