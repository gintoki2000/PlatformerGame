#ifndef CAMERA_H
#define CAMERA_H
#include "Vec.h"
#include "Rect.h"

class Camera
{
public:
    Camera();
    Camera(const Vec2& center, const Vec2& size);

    void Update();
    void SetCenter(const Vec2& center);
    Vec2 GetCenter() const;
    Vec2 GetTarget() const;
    void SetTarget(const Vec2 &target);

    Vec2 GetSize() const;
    void SetSize(const Vec2 &size);

    Rect GetViewport() const;

private:
    Vec2 m_center;
    Vec2 m_size;
    Vec2 m_target;
    mutable Rect m_viewport;
    mutable bool m_needToUpdateViewport;
};

#endif // CAMERA_H
