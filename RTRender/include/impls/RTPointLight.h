#ifndef RT_POINT_LIGHT_H
#define RT_POINT_LIGHT_H


#include "interfaces\RTLight.h"


namespace RTRender
{

	class CPointLight : public ILight
	{
		public:
			CPointLight(const CPoint& pos,const CColor& intensity);
			~CPointLight();

			CPoint GetPos() const;
			CColor GetIntensity() const;
		private:
			CPointLight();
	};
}

#endif