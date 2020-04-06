#ifndef CAMERA_H
#define CAMERA_H
#include "NTRect.h"
class Camera 
{
	public:
		Camera(int width, int height);

		void setTarget(float x, float y);
		void tick();

		const NTRect& getViewport() const;
	private:
		float m_centerX;
		float m_centerY;

		NTRect& m_viewport;
};
#endif //CAMERA_H
