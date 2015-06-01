#include "impls\RTBoundingBox.h"


namespace RTRender
{

	CBoundingBox::CBoundingBox()
	{
		min = CPoint(CMath::mInfinityF, CMath::mInfinityF, CMath::mInfinityF);
		max = CPoint(-CMath::mInfinityF, -CMath::mInfinityF, -CMath::mInfinityF);
	}

	CBoundingBox::CBoundingBox(const CBoundingBox& bbox):
		min(bbox.min), max(bbox.max)
	{
	}
	CBoundingBox::CBoundingBox(const CPoint& p0, const CPoint& p1) 
	{
		min = CPoint(CMath::Minf(p0.x, p1.x), CMath::Minf(p0.y, p1.y), CMath::Minf(p0.z, p1.z));
		max = CPoint(CMath::Maxf(p0.x, p1.x), CMath::Maxf(p0.y, p1.y), CMath::Maxf(p0.z, p1.z));
	}

	CBoundingBox::~CBoundingBox()
	{
	}

	CBoundingBox CBoundingBox::Union(const CPoint& p)
	{
		CBoundingBox resBBox = *this;

		F eps = CMath::mEpsilon;

		resBBox.min.x = CMath::Minf(min.x, p.x) - eps;
		resBBox.min.y = CMath::Minf(min.y, p.y) - eps;
		resBBox.min.z = CMath::Minf(min.z, p.z) - eps;

		resBBox.max.x = CMath::Maxf(max.x, p.x) + eps;
		resBBox.max.y = CMath::Maxf(max.y, p.y) + eps;
		resBBox.max.z = CMath::Maxf(max.z, p.z) + eps;

		return resBBox;
	}

	CBoundingBox CBoundingBox::Union( const CBoundingBox& bbox1)
	{
		CBoundingBox resBBox = *this;

		resBBox.min.x = CMath::Minf(min.x, bbox1.min.x);
		resBBox.min.y = CMath::Minf(min.y, bbox1.min.y);
		resBBox.min.z = CMath::Minf(min.z, bbox1.min.z);

		resBBox.max.x = CMath::Maxf(max.x, bbox1.max.x);
		resBBox.max.y = CMath::Maxf(max.y, bbox1.max.y);
		resBBox.max.z = CMath::Maxf(max.z, bbox1.max.z);

		return resBBox;
	}

	bool CBoundingBox::Inside(const CPoint& p) const
	{
		return (p.x >= min.x && p.x <= max.x &&
				p.y >= min.y && p.y <= max.y &&
				p.z >= min.z && p.z <= max.z);
	}

	bool CBoundingBox::Intersect(const CRay& ray, F& tmin, F& tmax) const
	{
		F ox = ray.origin.x; 
		F oy = ray.origin.y;
		F oz = ray.origin.z;

		F dx = ray.dir.x;
		F dy = ray.dir.y;
		F dz = ray.dir.z;

		F txMin, tyMin, tzMin;
		F txMax, tyMax, tzMax;

		F a = 1.0 / dx;

		if (a >= 0)
		{
			txMin = (min.x - ox) * a;
			txMax = (max.x - ox) * a;
		}
		else
		{
			txMin = (max.x - ox) * a;
			txMax = (min.x - ox) * a;
		}

		F b = 1.0 / dy;

		if (b >= 0)
		{
			tyMin = (min.y - oy) * b;
			tyMax = (max.y - oy) * b;
		}
		else
		{
			tyMin = (max.y - oy) * b;
			tyMax = (min.y - oy) * b;
		}

		F c = 1.0 / dz;

		if (c >= 0)
		{
			tzMin = (min.z - oz) * c;
			tzMax = (max.z - oz) * c;
		}
		else
		{
			tzMin = (max.z - oz) * c;
			tzMax = (min.z - oz) * c;
		}

		F t0, t1;

		t0 = txMin > tyMin ? txMin : tyMin;
		t0 = tzMin > t0 ? tzMin : t0;

		t1 = txMax < tyMax ? txMax : tyMax;
		t1 = tzMax < t1 ? tzMax : t1;

		tmin = t0;
		tmax = t1;

		return (t0 < t1 && t1 > CMath::mEpsilon);
	}

	unsigned char CBoundingBox::GetIndexOfMaxAxis() const
	{
		CVector3 diag = max - min;

	/*	if (diag.x - CMath::mEpsilon > diag.y && diag.x - CMath::mEpsilon > diag.z)
			return 0;
		else if (diag.y - CMath::mEpsilon > diag.z)
			return 1;
		else
			return 2;
		*/
		if (diag.x > diag.y &&  diag.x > diag.z)
			return 0;
		else if (diag.y > diag.z)
			return 1;
		else
			return 2;
	}

	F CBoundingBox::GetSurfaceArea() const
	{
		F w = abs(max.x - min.x);
		F h = abs(max.y - min.y);
		F d = abs(max.z - min.z);

		return 2.0 * (w * h + h * d + w * d);
	}

	CPoint CBoundingBox::GetCenter() const
	{
		CVector3 diag = max - min;
		diag *= 0.5;

		return CPoint(min.x + diag.x, min.y + diag.y, min.z + diag.z);
	}
}