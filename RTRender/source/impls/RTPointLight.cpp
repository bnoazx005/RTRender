#include "impls\RTPointLight.h"


namespace RTRender
{

	CPointLight::CPointLight()
	{
	}

	CPointLight::CPointLight(const CPoint& pos, const CColor& intensity):
		ILight(pos, intensity)
	{
	}

	CPointLight::~CPointLight()
	{
	}

	CPoint CPointLight::GetPos() const
	{
		return mPos;
	}

	CColor CPointLight::GetIntensity() const
	{
		return mIntensity;
	}
}