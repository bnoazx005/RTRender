#include "impls\RTConfig.h"
#include "impls\RTColor.h"
#include "impls\RTMath.h"
#include "impls\RTMaterial.h"
#include <iostream>


namespace RTRender
{

	CConfig::CConfig()
	{
	}

	CConfig::~CConfig()
	{
	}

	void CConfig::ParsePoint(const TiXmlElement* element, const std::string& tagName, CPoint& point)
	{
		std::string posStr = element->Attribute(tagName.c_str());

		unsigned char firstDelim = posStr.find_first_of(';');
		unsigned char secondDelim = posStr.find_first_of(';', firstDelim + 1);

		point.x = atof(posStr.substr(0, firstDelim).c_str());
		point.y = atof(posStr.substr(firstDelim + 1, secondDelim).c_str());
		point.z = atof(posStr.substr(secondDelim + 1, posStr.size()).c_str());
	}

	void CConfig::ParseVector(const TiXmlElement* element, const std::string& tagName, CVector3& vec3)
	{
		std::string posStr = element->Attribute(tagName.c_str());

		unsigned char firstDelim = posStr.find_first_of(';');
		unsigned char secondDelim = posStr.find_first_of(';', firstDelim + 1);

		vec3.x = atof(posStr.substr(0, firstDelim).c_str());
		vec3.y = atof(posStr.substr(firstDelim + 1, secondDelim).c_str());
		vec3.z = atof(posStr.substr(secondDelim + 1, posStr.size()).c_str());
	}

	void CConfig::ParseColor(const TiXmlElement* element, const std::string& tagName, CColor& color)
	{
		std::string posStr = element->Attribute(tagName.c_str());

		unsigned char firstDelim = posStr.find_first_of(';');
		unsigned char secondDelim = posStr.find_first_of(';', firstDelim + 1);

		color.r = atof(posStr.substr(0, firstDelim).c_str());
		color.g = atof(posStr.substr(firstDelim + 1, secondDelim).c_str());
		color.b = atof(posStr.substr(secondDelim + 1, posStr.size()).c_str());
	}

}