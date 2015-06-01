#include "impls\RTTriangle.h"
#include "impls\RTBoundingBox.h"


namespace RTRender
{

	CTriangle::CTriangle()
	{
	}

	CTriangle::CTriangle(const CPoint& p0, const CPoint& p1, const CPoint& p2, const TMaterial* material, const CColor& color) :
		mP0(p0), mP1(p1), mP2(p2), ISceneObject(material, color)
	{
		CVector3 u = mP1 - mP0;
		CVector3 v = mP2 - mP0;

		mNormal = CNormal(u.CrossProduct(v));
	}

	CTriangle::~CTriangle()
	{
	}

	/*!
               \brief This method uses the code by Kevin Suffern 'Ray Tracing From the Ground Up'
               Released under the GNU 2.0 License.
	*/

	bool CTriangle::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		CRay castedRay = ray;

		F a = mP0.x - mP1.x, b = mP0.x - mP2.x, c = castedRay.dir.x, d = mP0.x - castedRay.origin.x;
		F e = mP0.y - mP1.y, f = mP0.y - mP2.y, g = castedRay.dir.y, h = mP0.y - castedRay.origin.y;
		F i = mP0.z - mP1.z, j = mP0.z - mP2.z, k = castedRay.dir.z, l = mP0.z - castedRay.origin.z;

		F m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		F q = g * i - e * k, s = e * j - f * i;

		F invDenom = 1.0 / (a * m + b * q + c * s);

		F e1 = d * m - b * n - c * p;
		F beta = e1 * invDenom;

		if (beta < 0.0)
			return (false);

		F r = r = e * l - h * i;
		F e2 = a * n + d * q + c * r;
		F gamma = e2 * invDenom;

		if (gamma < 0.0)
			return false;

		if (beta + gamma > 1.0)
			return false;

		F e3 = a * p - b * r + d * s;
		F t0 = e3 * invDenom;

		if (t0 < CMath::mEpsilon)
			return false;

		t = t0;

		info.mNormal = mNormal;
		info.mRay = castedRay;
		info.mColor = mColor;
		info.mPoint = castedRay.origin + castedRay.dir * t;
		info.mpObject = this;
		info.mpMaterial = mpMaterial;

		return true;
	}

	CBoundingBox CTriangle::GetBoundingBox() const
	{
		CBoundingBox triBBox(mP0, mP1);
		triBBox = triBBox.Union(mP2);

		return triBBox;
	}

	bool CTriangle::IsInsideBBox(const CBoundingBox& bbox) const
	{
		if (bbox.Inside(mP0) || bbox.Inside(mP1) || bbox.Inside(mP2))
			return true;

		return false;
	}

}