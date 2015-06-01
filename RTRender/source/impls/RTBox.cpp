#include "impls\RTBox.h"
#include "impls\RTBoundingBox.h"


namespace RTRender
{

	CBox::CBox() :
		mMin(0.0, 0.0, 0.0), mMax(1.0, 1.0, 1.0), ISceneObject()
	{
	}

	CBox::CBox(const CVector3& pos, const CPoint& min, const CPoint& max, const TMaterial& material, const CColor& color):
		mMin(min), mMax(max), ISceneObject(pos, material, color)
	{
		mpBBox = new CBoundingBox(mMin, mMax);
	}

	CBox::CBox(const CVector3& pos, const F& width, const F& height, const TMaterial& material, const CColor& color):
		ISceneObject(pos, material, color)
	{
		/*F halfW = 0.5 * width;
		F halfH = 0.5 * height;

		CVector3 diag(halfW, halfH, sqrt(halfW * halfW + halfH * halfH));

		mMin = mPos - diag;
		mMax = mPos + diag;*/

		mpBBox = new CBoundingBox();
	}

	CBox::~CBox()
	{
	}

	bool CBox::Intersect(const CRay& ray, F& t)
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
			txMin = (mMin.x - ox) * a;
			txMax = (mMax.x - ox) * a;
		}
		else
		{
			txMin = (mMax.x - ox) * a;
			txMax = (mMin.x - ox) * a;
		}

		F b = 1.0 / dy;

		if (b >= 0)
		{
			tyMin = (mMin.y - oy) * b;
			tyMax = (mMax.y - oy) * b;
		}
		else
		{
			tyMin = (mMax.y - oy) * b;
			tyMax = (mMin.y - oy) * b;
		}

		F c = 1.0 / dz;

		if (c >= 0)
		{
			tzMin = (mMin.z - oz) * c;
			tzMax = (mMax.z - oz) * c;
		}
		else
		{
			tzMin = (mMax.z - oz) * c;
			tzMax = (mMin.z - oz) * c;
		}

		F t0, t1;
		int faceIn, faceOut;

		if (txMin > tyMin)
		{
			t0 = txMin;
			faceIn = (a >= 0.0) ? 0 : 3;
		}
		else
		{
			t0 = tyMin;
			faceIn = (b >= 0.0) ? 1 : 4;
		}

		if (tzMin > t0)
		{
			t0 = tzMin;
			faceIn = (c >= 0.0) ? 2 : 5;
		}

		if (txMax < tyMax)
		{
			t1 = txMax;
			faceOut = (a >= 0.0) ? 3 : 0;
		}
		else
		{
			t1 = tyMax;
			faceOut = (b >= 0.0) ? 4 : 1;
		}

		if (tzMax < t1)
		{
			t1 = tzMax;
			faceOut = (c >= 0.0) ? 5 : 2;
		}

		if (t0 < t1 && t1 > CMath::mEpsilon)
		{
			if (t0 > CMath::mEpsilon)
				t = t0;
			else
				t = t1;

			return true;
		}

		return false;
	}

	CColor CBox::GetColor(const CVector3& point)
	{
		return mColor;
	}

	CVector3 CBox::GetNormal(const CVector3& point)
	{
		return CVector3(0.0, 0.0, -1.0);
		//return mNormal;
	}

}