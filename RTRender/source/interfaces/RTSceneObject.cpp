#include "interfaces\RTSceneObject.h"
#include "impls\RTTransform.h"


namespace RTRender
{
	ISceneObject::ISceneObject()
	{
	}

	ISceneObject::ISceneObject(const TMaterial* material, const CColor& color) :
		mpMaterial(material), mColor(color)
	{
	}

	ISceneObject::~ISceneObject()
	{
	}

	const TMaterial* ISceneObject::GetMaterial() const
	{
		return mpMaterial;
	}
}