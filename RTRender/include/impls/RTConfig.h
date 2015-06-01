#ifndef RT_CONFIG_H
#define RT_CONFIG_H


#include "tinyxml\tinyxml.h"
#include <string>


namespace RTRender
{
	class CPoint;
	class CVector3;
	class CColor;


	class CConfig
	{
		public:
			CConfig();
			~CConfig();

			static void ParsePoint(const TiXmlElement* element, const std::string& tagName, CPoint& point);
			static void ParseVector(const TiXmlElement* element, const std::string& tagName, CVector3& vec3);
			static void ParseColor(const TiXmlElement* element, const std::string& tagName, CColor& color);
	};
}

#endif