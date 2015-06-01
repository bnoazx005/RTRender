#ifndef RT_LIGHT_H
#define RT_LIGHT_H


#include "impls\RTMath.h"
#include "impls\RTColor.h"


namespace RTRender
{

	enum E_LIGHT_TYPE
	{
		LT_POINT,
	};


	class ILight
	{
	public:
		virtual ~ILight() {}

		virtual CPoint GetPos() const = 0;
		virtual CColor GetIntensity() const = 0;
		protected:
			ILight()
			{
			}

			ILight(const CPoint& pos, const CColor& color) :
				mPos(pos), mIntensity(color)
			{
			}
		protected:
			CColor mIntensity;
			CPoint mPos;
	};
}

#endif