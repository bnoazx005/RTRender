#include "impls\RTSphere.h"
#include "impls\RTBoundingBox.h"


namespace RTRender
{

	CSphere::CSphere()
	{
	}

	CSphere::CSphere(const CPoint& center,const F& radius, const TMaterial* material, const CColor& color) :
		ISceneObject(material, color), mCenter(center), mRadius(radius)
	{
	}

	CSphere::~CSphere()
	{
	}

	bool CSphere::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		CRay r(ray);

		CVector3 dist = r.origin - mCenter;
		F b = dist.DotProduct(ray.dir);
		F c = dist.DotProduct(dist) - mRadius * mRadius;
		F d = b * b - c;

		if (d < 0)
			return false;

		F sqrtd = sqrt(d);

		F t0 = -b - sqrtd;
		F t1 = -b + sqrtd;

		F min_t = CMath::Minf(t0, t1);
		F max_t = CMath::Maxf(t0, t1);

		t = (min_t >= 0.0) ? min_t : max_t;

		info.mPoint = r.origin + r.dir * t;
		info.mColor = mColor;
		info.mRay = r;
		info.mNormal = CNormal(info.mPoint - mCenter);
		info.mpObject = this;
		info.mpMaterial = mpMaterial;

		return t > CMath::mEpsilon; //there was some rounding error here
	}

	CBoundingBox CSphere::GetBoundingBox() const
	{
		return CBoundingBox(CPoint(mCenter.x - mRadius, mCenter.y - mRadius, mCenter.z - mRadius),
			                CPoint(mCenter.x + mRadius, mCenter.y + mRadius, mCenter.z + mRadius));
	}

	bool CSphere::IsInsideBBox(const CBoundingBox& bbox) const
	{
		F x = mCenter.x, y = mCenter.y, z = mCenter.z;
		F r = mRadius;

		CPoint p0(x - r, y - r, z - r);
		CPoint p1(x + r, y + r, z + r);

		if (bbox.Inside(mCenter) || bbox.Inside(p0) || bbox.Inside(p1))
			return true;

		return false;
	}
}