#include "interfaces\RTCamera.h"
#include "impls\RTTGAImage.h"
#include "impls\RTPPMImage.h"


namespace RTRender
{

	ICamera::ICamera()
	{
	}

	ICamera::ICamera(const CPoint& position, const CPoint& target, unsigned int width, unsigned int height, E_IMAGE_TYPE type):
		mPosition(position), mTarget(target), mWidth(width), mHeight(height)
	{
		switch (type)
		{
			case IT_TGA:
				mpImageBuffer = new CTGAImage(width, height, TIT_TRUE_COLOR_24);
				break;
			case IT_PPM:
				mpImageBuffer = new CPPMImage(width, height);
				break;
		}
	}

	ICamera::~ICamera()
	{
		if (mpImageBuffer)
			delete mpImageBuffer;
	}

	void ICamera::SetPosition(const CPoint& position)
	{
		mPosition = position;
		//matrix of camera should be recomputed
	}

	void ICamera::SetTarget(const CPoint& target)
	{
		mTarget = target;
		//matrix of camera should be recomputed
	}

	CPoint ICamera::GetPosition() const
	{
		return mPosition;
	}

	CPoint ICamera::GetTarget() const
	{
		return mTarget;
	}

	IImage* ICamera::GetImageBuffer()
	{
		return mpImageBuffer;
	}
}