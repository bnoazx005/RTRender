#ifndef RT_SCENE_OBJECT_H
#define RT_SCENE_OBJECT_H


#include "impls\RTMaterial.h"
#include "impls\RTColor.h"
#include "impls\RTTransform.h"


namespace RTRender
{
	class ISceneObject;

	#pragma pack(push, 1)

	struct THitInfo
	{
		CRay mRay;
		CNormal mNormal;
		CPoint mPoint;
		CColor mColor;

		const ISceneObject* mpObject;
		const TMaterial* mpMaterial;
	};

	#pragma pack(pop)


	enum E_PRIMITIVE_TYPE
	{
		PT_TRIANGLE,
		PT_SPHERE,
		PT_MESH_TRIANGLE,
	};

	class ISceneObject
	{
		public:
			ISceneObject(const TMaterial* material, const CColor& color);
			virtual ~ISceneObject();

			virtual bool Hit(const CRay& ray, F& t, THitInfo& info) const = 0;

			virtual bool IsInsideBBox(const CBoundingBox& bbox) const = 0;

			const TMaterial* GetMaterial() const;

			virtual CBoundingBox GetBoundingBox() const= 0;
		protected:
			ISceneObject();
		protected:
			const TMaterial* mpMaterial;

			CColor mColor;
	};
}

#endif