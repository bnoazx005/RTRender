#ifndef RT_SPHERE_H
#define RT_SPHERE_H


#include "interfaces\RTSceneObject.h"


namespace RTRender
{

	class CSphere : public ISceneObject
	{
		public:
			CSphere(const CPoint& center,const F& radius, const TMaterial* material, const CColor& color);
			~CSphere();
			
			bool Hit(const CRay& ray, F& t, THitInfo& info) const;

			bool IsInsideBBox(const CBoundingBox& bbox) const;

			CBoundingBox GetBoundingBox() const;
		private:
			CSphere();
		private:
			CPoint mCenter;
			F mRadius;
	};
}

#endif