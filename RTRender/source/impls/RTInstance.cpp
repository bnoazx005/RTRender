#include "impls\RTInstance.h"


namespace RTRender
{
	CInstance::CInstance()
	{
	}

	CInstance::CInstance(const CTransform* w2o, const CTransform* o2w, const ISceneObject* object,
		                 const TMaterial* material, const CColor& color) :
		mpWorldToObject(w2o), mpObjectToWorld(o2w), mpObject(object), ISceneObject(material, color)
	{
	}

	CInstance::~CInstance()
	{
	}

	bool CInstance::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		//use invertedTransorm to ray
		CRay transformedRay = (*mpObjectToWorld)(ray);

		bool isIntersect = mpObject->Hit(transformedRay, t, info);

		if (!isIntersect)
			return false;

		info.mNormal = (*mpObjectToWorld)(info.mNormal);
		info.mColor = mColor;
		info.mpObject = this;

		return true;
	}

	CBoundingBox CInstance::GetBoundingBox() const
	{
		return mpObject->GetBoundingBox();
	}

	bool CInstance::IsInsideBBox(const CBoundingBox& bbox) const
	{
		return mpObject->IsInsideBBox(bbox);
	}

}