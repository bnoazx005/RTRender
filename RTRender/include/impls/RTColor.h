#ifndef RT_COLOR_H
#define RT_COLOR_H


#include "RTTypes.h"


namespace RTRender
{
	#pragma pack(push, 1)

		struct TR8G8B8Color
		{
			unsigned char mR, mG, mB;
		};

	#pragma pack(pop)


	class CColor
	{
		public:
			CColor();

			CColor(const CColor& color);

			CColor(F32 r, F32 g, F32 b);

			~CColor();
 
			CColor operator+ (const CColor& color) const;
			void operator+= (const CColor& color);
			CColor operator* (const F32& value) const;
			CColor operator* (const CColor& color) const;

			CColor ToRange();
			TR8G8B8Color ToR8G8B8Color() const;
		public:
			F32 r, g, b;
	};
}

#endif