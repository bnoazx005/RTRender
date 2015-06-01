#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H


#include "interfaces\RTSceneObject.h"


namespace RTRender
{

	class CTriangle : public ISceneObject
	{
		public:
			CTriangle(const CPoint& p0, const CPoint& p1, const CPoint& p2, const TMaterial* material, const CColor& color);
			~CTriangle();

			bool Hit(const CRay& ray, F& t, THitInfo& info) const;

			bool IsInsideBBox(const CBoundingBox& bbox) const;

			CBoundingBox GetBoundingBox() const;
		private:
			CTriangle();
		private:
			CPoint mP0, mP1, mP2;
			CNormal mNormal;
	};
		
}

#endif