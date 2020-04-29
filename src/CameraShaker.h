#ifndef CAMERA_SHAKER_H
#define CAMERA_SHAKER_H
#include "Camera.h"
class CameraShaker
{
  public:

    static CameraShaker *create(Camera* camera);

    void shake(float duration, int amplitude, int frequency);
    void tick(float deltaTime);

  private:
    CameraShaker();
    bool init(Camera* camera);
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
