#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H


#include "RTMath.h"


namespace RTRender
{

	struct TMaterial
	{
		F mDiffuse;
		F mReflection;
		F mSpecular;
		F mTransparency;
		F mRefractCoeff;
	};
}

#endif