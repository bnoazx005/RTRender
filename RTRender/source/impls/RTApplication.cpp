#include "impls\RTApplication.h"
#include <iostream>
#include "tinyxml\tinyxml.h"
#include "impls\RTScene.h"
#include "impls\RTPerspectiveCamera.h"
#include "impls\RTRayTracer.h"
#include "impls\RTMPIRayTracer.h"
#include "impls\RTOMPRayTracer.h"
#include "impls\RTConfig.h"


namespace RTRender
{
	CApplication::CApplication() :
		mIsReady(false), mName("")
	{
	}

	CApplication::~CApplication()
	{
		delete mpTracer;
		delete mpScene;
		delete mpCamera;
	}

	bool CApplication::Init(const std::string& name, int commandLineArgsCount, char** commandLineArgs)
	{
		if (mIsReady)
			return false;

		mName = name;

		std::cout << ">> Initialization is started\n";
		std::cout << ">> Reading of configuration file...\n";

		TiXmlDocument* pConfig = new TiXmlDocument("config.xml");

		if (!pConfig->LoadFile())
		{
			std::cerr << "Error: Incorrect configururation file\n";
			std::cerr << pConfig->ErrorDesc() << "\n";

			return false;
		}

		TiXmlElement* pConfigNode = pConfig->FirstChildElement("config");

		if (!pConfigNode)
		{
			std::cerr << "Error: A <config> section wasn't found\n";
			return false;
		}

		TiXmlElement* pParamNode = pConfigNode->FirstChildElement("render");

		//render section

		if (!pParamNode)
		{
			std::cerr << "Error: Section <render> wasn't found\n";
			return false;
		}

		E_TRACER_TYPE tracerType;

		std::string typeStr = pParamNode->Attribute("type");

		if (typeStr == "OMP")
			tracerType = TT_OMP;
		else if (typeStr == "MPI")
			tracerType = TT_MPI;
		else
			tracerType = TT_SEQUENTIAL;

		int schemeId = 1;
		pParamNode->Attribute("scheme", &schemeId);

		if (schemeId < 1 || schemeId > 2)
		{
			std::cerr << "Error: Incorrect value of scheme in render section\n";
			return false;
		}

		int maxDepth = 0;
		pParamNode->Attribute("max_depth", &maxDepth);

		if (maxDepth <= 0)
		{
			std::cerr << "Error: Incorrect value of max_depth attribute in render section\n";
			return false;
		}

		mMaxDepth = maxDepth;

		//end of render section

		pParamNode = pConfigNode->FirstChildElement("camera");

		//camera section

		if (!pParamNode)
		{
			std::cerr << "Error: Section <camera> wasn't found\n";
			return false;
		}

		E_CAMERA_TYPE cameraType;

		std::string cameraTypeStr = pParamNode->Attribute("type");

		if (cameraTypeStr == "perspective")
			cameraType = CT_PERSPECTIVE;
		else
		{
			std::cerr << "Error: Unknown type of camera\n";
			return false;
		}
		
		CPoint cameraPos;
		CConfig::ParsePoint(pParamNode, "pos", cameraPos);

		CPoint targetPos;
		CConfig::ParsePoint(pParamNode, "target", targetPos);

		F fov = 0.0;
		pParamNode->Attribute("fov", &fov);

		if (fov < CMath::mEpsilon || fov >= 90.0)
		{
			std::cerr << "Error: Incorrect value of field of view\n";
			return false;
		}

		int width, height;

		pParamNode->Attribute("width", &width);
		pParamNode->Attribute("height", &height);

		if (width <= 0 || height <= 0)
		{
			std::cerr << "Error: Incorrect sizes of an image rectangle\n";
			return false;
		}

		//end of camera section

		//result section

		E_IMAGE_TYPE imageType;

		pParamNode = pConfigNode->FirstChildElement("result");

		if (!pParamNode)
		{
			std::cerr << "Error: Section <result> wasn't found\n";
			return false;
		}

		std::string imageTypeStr = pParamNode->Attribute("type");

		if (imageTypeStr == "tga")
			imageType = IT_TGA;
		else if (imageTypeStr == "ppm")
			imageType = IT_PPM;

		std::string resultFilePath = pParamNode->Attribute("path");
		std::string resultName = pParamNode->Attribute("name");

		std::string fullImageName = resultFilePath + "\\" + resultName + "." + imageTypeStr;

		//end of result section

		pParamNode = pConfigNode->FirstChildElement("scene");

		//scene section

		if (!pParamNode)
		{
			std::cerr << "Error: Section <scene> wasn't found\n";
			return false;
		}

		std::string sceneFilePath = pParamNode->Attribute("path");

		CColor backgroundColor;
		CConfig::ParseColor(pParamNode, "bg_color", backgroundColor);

		int bvhMaxDepth = 0;

		pParamNode = pParamNode->FirstChildElement("bvh");
		std::string bvhUsingStr = pParamNode->Attribute("use");

		if (bvhUsingStr == "true")
		{
			pParamNode->Attribute("max_depth", &bvhMaxDepth);

			if (bvhMaxDepth <= 0)
			{
				std::cerr << "Error: Incorrect value of max depth for BVH\n";
				return false;
			}

			std::cout << ">> BVH tree is using\n";
		}

		//end of scene section

		delete pConfig;

		std::cout << ">> Configuration file is read\n";

		switch (tracerType)
		{
			case TT_OMP:
				mpTracer = new COMPRayTracer(schemeId);
				std::cout << ">> Ray tracer uses OpenMP mode\n";
				break;
			case TT_MPI:
				mpTracer = new CMPIRayTracer(commandLineArgsCount, commandLineArgs, schemeId);
				std::cout << ">> Ray tracer uses MPI mode\n";
				break;
			case TT_SEQUENTIAL:
				mpTracer = new CRayTracer();
				std::cout << ">> Ray tracer uses sequential mode\n";
				break;
		}

		switch (cameraType)
		{
			case CT_PERSPECTIVE:
				mpCamera = new CPerspectiveCamera(cameraPos, targetPos, fov, width, height, imageType);
				mpCamera->GetImageBuffer()->SetFilename(fullImageName);
				break;
		}

		mpScene = new CScene(backgroundColor);
		
		std::cout << ">> Scene is being loaded\n";

		if (!mpScene->LoadFromFile(sceneFilePath))
			return false;

		std::cout << ">> Scene is sucessfully loaded\n";

		if (bvhUsingStr == "true")
		{
			std::cout << ">> BVH Tree is being built now\n";

			mpScene->PreCompute(bvhMaxDepth);

			std::cout << ">> BVH Tree was built\n";
		}

		std::cout << ">> Initialization is finished\n";
		std::cout << ">> Total count of primitives: " << mpScene->GetPrimitivesCount() << std::endl;

		mIsReady = true;

		return true;
	}

	void CApplication::Run()
	{
		if (!mIsReady)
			return;

		std::cout << ">> Ray tracer starts the work...\n";

		mpTracer->Render(mpCamera, mpScene, mMaxDepth);

		std::cout << ">> Ray tracer finished the work\n";
	}

	std::string CApplication::GetName() const
	{
		return mName;
	}
}