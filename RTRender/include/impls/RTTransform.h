#ifndef RT_TRANSFORM_H
#define RT_TRANSFORM_H


#include "RTMath.h"
#include "RTBoundingBox.h"


namespace RTRender
{

	class CTransform
	{
		public:
			CTransform();
			CTransform(const CMatrix4x4& mat);
			CTransform(const CMatrix4x4& mat, const CMatrix4x4& invMat);
			~CTransform();

			void Inverse();

			//some transformations
			CTransform Translate(const CVector3& translationVec);
			CTransform Scale(const CVector3& scalingVec);
			CTransform RotateX(const F& angle);
			CTransform RotateY(const F& angle);
			CTransform RotateZ(const F& angle);
			CTransform Perspective(const F& fov, const F& zn, const F& zf);

			CPoint operator() (const CPoint& point) const;
			CVector3 operator() (const CVector3& vec) const;
			CNormal operator() (const CNormal& n) const;
			CRay operator() (const CRay& ray) const;
			CBoundingBox operator() (const CBoundingBox& bbox) const;

			CTransform operator* (const CTransform& transf) const;
		private:
			CMatrix4x4 mTransform, mInvTransform;
	};
}

#endif