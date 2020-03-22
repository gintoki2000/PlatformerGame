#ifndef BUILDER_H
#define BUILDER_H
#include "box2d/box2d.h"
#include "tmxlite/Object.hpp"
#include "tmxlite/Types.hpp"
#include <vector>
class Builder
{
  public:
    static b2PolygonShape*
                           buildPolygon(const std::vector<tmx::Vector2f>& points);
    static b2PolygonShape* buildRectangle(const tmx::Rectangle<float>& rect);
    static b2Shape*        buildShape(const tmx::Object& object);

  private:
    Builder() {}
};
#endif // BUILDER_H
