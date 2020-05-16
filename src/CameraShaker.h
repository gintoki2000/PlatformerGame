#ifndef CAMERA_SHAKER_H
#define CAMERA_SHAKER_H
#include "Camera.h"
class CameraShaker
{
  public:

    static CameraShaker *Create(Camera* camera);

    void Shake(float duration, int amplitude, int frequency);
    void Tick(float deltaTime);

  private:
    CameraShaker();
    bool Init(Camera* camera);
    Camera* m_camera;
    float   m_duration;
    int     m_amplitude;
    float   m_waitTime;
    bool    m_isShaking;
    float   m_timer;
    float   m_waitTimer;
    float   m_dx;
    float   m_dy;
    float   m_initialOffsetX;
    float   m_initialOffsetY;
};
#endif // CAMERA_SHAKER_H
