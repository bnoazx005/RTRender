#ifndef RT_PLANE_H
#define RT_PLANE_H


#include "interfaces\RTSceneObject.h"


namespace RTRender 
{

	class CPlane : public ISceneObject
	{
		public:
			CPlane(const CPoint& pos,const CNormal& normal, const TMaterial* material, const CColor& color);
			~CPlane();

			bool Hit(const CRay& ray, F& t, THitInfo& info) const;

			bool IsInsideBBox(const CBoundingBox& bbox) const;

			CBoundingBox GetBoundingBox() const;
		private:
			CPlane();
		private:
			CPoint mPos;
			CNormal mNormal;
	};
}

#endif