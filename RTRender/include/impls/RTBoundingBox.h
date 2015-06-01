#ifndef RT_BOUNDING_BOX_H
#define RT_BOUNDING_BOX_H


#include "RTMath.h"


namespace RTRender
{

	class CBoundingBox
	{
		public:
			CBoundingBox();
			CBoundingBox(const CBoundingBox& bbox);
			CBoundingBox(const CPoint& p0, const CPoint& p1);
			~CBoundingBox();

			CBoundingBox Union(const CPoint& p);
			CBoundingBox Union(const CBoundingBox& bbox1);

			bool Inside(const CPoint& p) const;

			bool Intersect(const CRay& ray, F& tmin, F& tmax) const;

			unsigned char GetIndexOfMaxAxis() const;
			F GetSurfaceArea() const;

			CPoint GetCenter() const;
		public:
			CPoint min, max;
	};
}

#endif