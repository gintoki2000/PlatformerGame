#ifndef CAMERA_H
#define CAMERA_H
#include "Vec.h"
#include "Rect.h"

class Camera
{
public:
    Camera();
    Camera(const Vec2& center, const Vec2& size);

    void update();
    void setCenter(const Vec2& center);
    Vec2 getCenter() const;
    Vec2 getTarget() const;
    void setTarget(const Vec2 &target);

    Vec2 getSize() const;
    void setSize(const Vec2 &size);

    Rect getViewport() const;

private:
    Vec2 m_center;
    Vec2 m_size;
    Vec2 m_target;
    mutable Rect m_viewport;
    mutable bool m_needToUpdateViewport;
};

#endif // CAMERA_H
