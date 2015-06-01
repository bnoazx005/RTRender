#include "impls\RTPerspectiveCamera.h"


namespace RTRender
{
	CPerspectiveCamera::CPerspectiveCamera(const CPoint& position, const CPoint& target, const F& fov, unsigned int width, unsigned int height,
		                                   E_IMAGE_TYPE type) :
		ICamera(position, target, width, height, type), mFOV(fov)
	{
		computeTransform();
	}

	CPerspectiveCamera::~CPerspectiveCamera()
	{
	}

	CRay CPerspectiveCamera::ComputeRay(const F& x, const F& y)
	{
		CRay ray;
		
		ray.origin = mPosition;

		ray.dir = mU * (-0.5 * mNearPlaneWidth + (x + 0.5) * mTransformCoeffX) +
		          mV * (0.5 * mNearPlaneHeight - (y + 0.5) * mTransformCoeffY) -
		          mW;

		ray.dir.Normalize();

		return ray;
	}

	void CPerspectiveCamera::SetPosition(const CPoint& position)
	{
		mPosition = position;
		computeTransform();
	}

	void CPerspectiveCamera::SetTarget(const CPoint& target)
	{
		mTarget = target;
		computeTransform();
	}

	void CPerspectiveCamera::computeTransform()
	{
		/*CVector3 v = mTarget - mPosition;
		CVector3 r = v.CrossProduct(CVector3::mUp);
		CVector3 u = r.CrossProduct(v);

		v.Normalize();
		r.Normalize();
		u.Normalize();

		F aspect = F(mWidth / mHeight);
		F tanPhi = tan(mFOV * 0.5);

		u *= tanPhi;
		v *= (tanPhi * aspect);

		v = v - (r + u);
		r *= 2;
		v *= 2;

		CMatrix4x4 camToWorld(r.x, u.x, v.x, 0.0,
			r.y, u.y, v.y, 0.0,
			r.z, u.z, v.z, 0.0,
			0.0, 0.0, 0.0, 1.0);

		mCamToWorld = CTransform(camToWorld);*/

		mW = mPosition - mTarget;
		mU = mW.CrossProduct(CVector3::mUp);
		mV = mU.CrossProduct(mW);

		mU.Normalize();
		mV.Normalize();
		mW.Normalize();

		F mAspectRatio = F(mHeight) / mWidth;

		mNearPlaneWidth = 2.0 * tan(CMath::Deg2Rad(mFOV));
		mNearPlaneHeight = mNearPlaneWidth * mAspectRatio;
		
		mTransformCoeffX = mNearPlaneWidth / (F)mWidth;
		mTransformCoeffY = mNearPlaneHeight / (F)mHeight;
	}

	/*!
		CCamera
	*/

	CCamera::CCamera(const CPoint& eyePos, const CPoint& target, int screenWidth, int screenHeight, F fovy) :
		mScreenHeight(screenHeight), mScreenWidth(screenWidth), mEye(eyePos), mTarget(target), mFovy(fovy)
	{
		CVector3 d = mEye - mTarget;
		CVector3 up(CVector3::mUp);

		mU = d.CrossProduct(CVector3::mUp);
		mV = mU.CrossProduct(d);
		mW = d;

		mU.Normalize();
		mV.Normalize();
		mW.Normalize();

		mAspectRatio = (float)mScreenHeight / mScreenWidth;

		mNearPlaneWidth = 2.0 * tan(mFovy * 3.14 / 180); //distance to near plane equals 1.0
		mNearPlaneHeight = mNearPlaneWidth * mAspectRatio;

		mTransformCoeffX = mNearPlaneWidth / mScreenWidth;
		mTransformCoeffY = mNearPlaneHeight / mScreenHeight;
	}

	CCamera::~CCamera()
	{
	}

	CRay CCamera::GenerateRay(const F& xPixelCoord,const F& yPixelCoord)
	{
		CRay ray;

		ray.origin = mEye;

		ray.dir = mU * (-0.5 * mNearPlaneWidth + (xPixelCoord + 0.5) * mTransformCoeffX) +
				  mV * (0.5 * mNearPlaneHeight - (yPixelCoord + 0.5) * mTransformCoeffY) -
				  mW;
		
		ray.dir.Normalize();

		return ray;
	}
}