#include "impls\RTPlane.h"
#include "impls\RTBoundingBox.h"


namespace RTRender
{

	CPlane::CPlane()
	{
	}

	CPlane::CPlane(const CPoint& pos, const CNormal& normal, const TMaterial* material, const CColor& color) :
		ISceneObject(material, color), mPos(pos), mNormal(normal)
	{
	}

	CPlane::~CPlane()
	{
	}

	bool CPlane::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		CRay tRay = ray;

		F invDot = 1.0 / ray.dir.DotProduct(mNormal);
		CVector3 dist = mPos - ray.origin;
		F t0 = dist.DotProduct(mNormal) * invDot;

		if (t0 > CMath::mEpsilon)
		{
			t = t0;

			info.mNormal = mNormal;
			info.mPoint = tRay.origin + tRay.dir * t;
			info.mColor = mColor;
			info.mRay = tRay;
			info.mpObject = this;
			info.mpMaterial = mpMaterial;

			return true;
		}

		return false;
	}

	CBoundingBox CPlane::GetBoundingBox() const
	{
		return CBoundingBox(CPoint(-500.0, -0.5, -500.0), CPoint(500.0, 0.5, 500.0));
	}

	bool CPlane::IsInsideBBox(const CBoundingBox& bbox) const
	{
		return true;
	}

}