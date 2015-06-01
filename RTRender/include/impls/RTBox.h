#ifndef RT_BOX_H
#define RT_BOX_H


#include "interfaces\RTSceneObject.h"


namespace RTRender
{

	class CBox : public ISceneObject
	{
		public:
			CBox(const CVector3& pos, const CPoint& min, const CPoint& max, const TMaterial& material, const CColor& color);
			CBox(const CVector3& pos, const F& width, const F& height, const TMaterial& material, const CColor& color);
			~CBox();

			bool Intersect(const CRay& ray, F& t);
			CColor GetColor(const CVector3& point);
			CVector3 GetNormal(const CVector3& point);
		private:
			CBox();
		private:
			CPoint mMax, mMin;
	};
}

#endif