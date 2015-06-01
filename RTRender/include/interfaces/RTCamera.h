#ifndef RT_CAMERA_H
#define RT_CAMERA_H


#include "impls\RTMath.h"
#include "interfaces\RTImage.h"


namespace RTRender
{

	enum E_CAMERA_TYPE
	{
		CT_PERSPECTIVE
	};


	class ICamera
	{
		public:
			ICamera(const CPoint& position, const CPoint& target, unsigned int width, unsigned int height, E_IMAGE_TYPE type);
			virtual ~ICamera();

			virtual CRay ComputeRay(const F& x, const F& y) = 0;

			virtual void SetPosition(const CPoint& position);
			virtual void SetTarget(const CPoint& target);

			CPoint GetPosition() const;
			CPoint GetTarget() const;

			IImage* GetImageBuffer();
		private:
			ICamera();
		protected:
			CPoint mPosition;
			CPoint mTarget;

			unsigned int mWidth;
			unsigned int mHeight;

			IImage* mpImageBuffer;
	};
}

#endif