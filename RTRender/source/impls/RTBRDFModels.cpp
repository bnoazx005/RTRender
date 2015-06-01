#include "impls\RTBRDFModels.h"
#include "impls\RTMath.h"
#include "interfaces\RTSceneObject.h"


namespace RTRender
{

	CLambertianBRDF::CLambertianBRDF():
		mDiffuseCoeff(1.0), mDiffuseColor(1.0, 1.0, 1.0), IBRDF()
	{
	}

	CLambertianBRDF::CLambertianBRDF(const RTRender::F& diffuseCoeff, const CColor& diffuseColor) :
		mDiffuseCoeff(diffuseCoeff), mDiffuseColor(diffuseColor), IBRDF()
	{
	}

	CLambertianBRDF::~CLambertianBRDF()
	{
	}

	void CLambertianBRDF::SetDiffuseCoeff(const RTRender::F& coeff)
	{
		mDiffuseCoeff = coeff;
	}

	void CLambertianBRDF::SetDiffuseColor(const CColor& color)
	{
		mDiffuseColor = color;
	}

	CColor CLambertianBRDF::F(const THitInfo& info, const CVector3& i, const CVector3& o) const
	{
		return mDiffuseColor * (mDiffuseCoeff * CMath::mInvPi);
	}
}