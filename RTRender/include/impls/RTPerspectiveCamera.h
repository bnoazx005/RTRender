#ifndef RT_PERSPECTIVE_CAMERA_H
#define RT_PERSPECTIVE_CAMERA_H


#include "RTMath.h"
#include "interfaces\RTCamera.h"
#include "RTTransform.h"


namespace RTRender
{
	class CPerspectiveCamera : public ICamera
	{
		public:
			CPerspectiveCamera(const CPoint& position, const CPoint& target, const F& fov, unsigned int width, unsigned int height, 
				               E_IMAGE_TYPE type);
			~CPerspectiveCamera();

			CRay ComputeRay(const F& x, const F& y);

			void SetPosition(const CPoint& position);
		    void SetTarget(const CPoint& target);
		private:
			void computeTransform();
		private:
			F mFOV;

			CTransform mCamToWorld;

			F mAspectRatio;

			CVector3 mU, mV, mW; 

			F mNearPlaneWidth;
			F mNearPlaneHeight;

			F mTransformCoeffX;
			F mTransformCoeffY;
	};

	class CCamera
	{
		public:
			CCamera(const CPoint& eyePos, const CPoint& target, int screenWidth, int screenHeight, F fovy);
			~CCamera();

			CRay GenerateRay(const F& xPixelCoord,const F& yPixelCoord);
		private:
			CPoint mEye;
			CPoint mTarget;

			int mScreenWidth;
			int mScreenHeight;

			F mFovy;
			F mAspectRatio;

			CVector3 mU, mV, mW; ///basis of camera

			F mNearPlaneWidth;
			F mNearPlaneHeight;

			F mTransformCoeffX;
			F mTransformCoeffY;
	};
}

#endif