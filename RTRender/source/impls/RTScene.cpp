#include "impls\RTScene.h"
#include "impls\RTPointLight.h"
#include "impls\RTSphere.h"
#include "impls\RTTriangle.h"
#include "impls\RTOBJLoader.h"
#include "impls\RTMaterial.h"
#include "impls\RTBVHTree.h"
#include "tinyxml\tinyxml.h"
#include <iostream>
#include "impls\RTConfig.h"


namespace RTRender
{

	CScene::CScene():
		mBackgroundColor(0.0, 0.0, 0.0), mPrimitivesCount(0), mLightSourcesCount(0), mpBVHTree(nullptr)
	{
	}

	CScene::CScene(const CColor& backgroundColor) :
		mBackgroundColor(backgroundColor), mPrimitivesCount(0), mLightSourcesCount(0), mpBVHTree(nullptr)
	{
	}

	CScene::~CScene()
	{
		if (!mPrimitives.empty())
		{
			for (unsigned int i = 0; i < mPrimitivesCount; ++i)
				delete mPrimitives[i];
			
			mPrimitives.clear();
			mPrimitivesCount = 0;
		}

		if (!mLightSources.empty())
		{
			for (unsigned int i = 0; i < mLightSourcesCount; ++i)
				delete mLightSources[i];

			mLightSources.clear();
			mLightSourcesCount = 0;

		}

		if (!mMaterials.empty())
		{
			for (std::map<std::string, TMaterial*>::iterator i = mMaterials.begin(); i != mMaterials.end(); i++)
				delete i->second;

			mMaterials.clear();
		}

		if (!mMeshBuffers.empty())
		{
			for (std::vector<TMeshBuffer*>::iterator i = mMeshBuffers.begin(); i != mMeshBuffers.end(); i++)
				delete *i;

			mMeshBuffers.clear();
		}

		delete mpBVHTree;
	}

	bool CScene::LoadFromFile(const std::string& filename)
	{
		TiXmlDocument* pSceneFile = new TiXmlDocument(filename.c_str());

		if (!pSceneFile->LoadFile())
			return false;

		TiXmlElement* pSceneNode = pSceneFile->FirstChildElement("scene");

		if (!pSceneNode)
		{
			std::cerr << "Error: There is no <scene> entity in scene's configuration file\n";
			return false;
		}

		TiXmlElement* pMaterialsNode = pSceneNode->FirstChildElement("materials");
		TiXmlElement* pPrimitivesNode = pSceneNode->FirstChildElement("primitives");
		TiXmlElement* pLightsNode = pSceneNode->FirstChildElement("lights");

		if (!pMaterialsNode || !pPrimitivesNode || !pLightsNode)
		{
			std::cerr << "Error: Incorrect structure of scene file\n";
			return false;
		}

		TiXmlElement* pCurrMaterialNode = pMaterialsNode->FirstChildElement();

		std::string materialName;
		TMaterial* pCurrMaterial = nullptr;

		while (pCurrMaterialNode)
		{
			pCurrMaterial = new TMaterial();

			materialName = pCurrMaterialNode->Attribute("name");

			pCurrMaterialNode->Attribute("kd", &pCurrMaterial->mDiffuse);
			
			if (pCurrMaterial->mDiffuse < 0.0 || pCurrMaterial->mDiffuse > 1.0)
			{
				std::cerr << "Error: Incorrect value of kd in material (" + materialName + "). Kd should be in range of [0; 1]\n";
				return false;
			}

			pCurrMaterialNode->Attribute("kr", &pCurrMaterial->mReflection);

			if (pCurrMaterial->mReflection < 0.0 || pCurrMaterial->mReflection > 1.0)
			{
				std::cerr << "Error: Incorrect value of kr in material (" + materialName + "). Kr should be in range of [0; 1]\n";
				return false;
			}

			pCurrMaterialNode->Attribute("ks", &pCurrMaterial->mSpecular);

			if (pCurrMaterial->mSpecular < 0.0 || pCurrMaterial->mSpecular > 1.0)
			{
				std::cerr << "Error: Incorrect value of ks in material (" + materialName + "). Ks should be in range of [0; 1]\n";
				return false;
			}

			pCurrMaterialNode->Attribute("kt", &pCurrMaterial->mTransparency);

			if (pCurrMaterial->mTransparency < 0.0 || pCurrMaterial->mTransparency > 1.0)
			{
				std::cerr << "Error: Incorrect value of kt in material (" + materialName + "). Kt should be in range of [0; 1]\n";
				return false;
			}

			pCurrMaterialNode->Attribute("refract_coeff", &pCurrMaterial->mRefractCoeff);

			if (pCurrMaterial->mRefractCoeff < 0.0)
			{
				std::cerr << "Error: Incorrect value of refraction coefficient in material (" + materialName + ")\n";
				return false;
			}

			mMaterials.insert(std::make_pair(materialName, pCurrMaterial));

			pCurrMaterialNode = pCurrMaterialNode->NextSiblingElement();
		}


		//reading of lights from config file
		TiXmlElement* pCurrLightNode = pLightsNode->FirstChildElement();

		E_LIGHT_TYPE lightType;

		std::string lightTypeStr;

		CPoint lightPos;
		CColor lightColor;

		while (pCurrLightNode)
		{
			lightTypeStr = pCurrLightNode->Attribute("type");

			if (lightTypeStr == "point")
				lightType = LT_POINT;
			else
			{
				std::cerr << "Error: Incorrect type of light entity\n";
				return false;
			}

			CConfig::ParsePoint(pCurrLightNode, "pos", lightPos);
			CConfig::ParseColor(pCurrLightNode, "color", lightColor);

			switch (lightType)
			{
				case LT_POINT:
					AddPointLight(lightPos, lightColor);
					break;
			}

			pCurrLightNode = pCurrLightNode->NextSiblingElement();
		}

		//primitives
		TiXmlElement* pCurrPrimitiveNode = pPrimitivesNode->FirstChildElement();

		E_PRIMITIVE_TYPE primitiveType;

		std::string primitiveTypeStr;
		std::string meshFilePath;

		CPoint center;
		F radius;

		CPoint p0, p1, p2;

		CColor primitiveColor;

		while (pCurrPrimitiveNode)
		{
			primitiveTypeStr = pCurrPrimitiveNode->Attribute("type");

			if (primitiveTypeStr == "sphere")
				primitiveType = PT_SPHERE;
			else if (primitiveTypeStr == "triangle")
				primitiveType = PT_TRIANGLE;
			else if (primitiveTypeStr == "mesh")
				primitiveType = PT_MESH_TRIANGLE;
			else
			{
				std::cerr << "Error: Incorrect type of primitive\n";
				return false;
			}

			CConfig::ParseColor(pCurrPrimitiveNode, "color", primitiveColor);
			materialName = pCurrPrimitiveNode->Attribute("material");

			if (mMaterials.find(materialName) == mMaterials.end())
			{
				std::cerr << "Error: There is no the material with name (" + materialName + ")\n";
				return false;
			}

			pCurrMaterial = mMaterials.find(materialName)->second;

			switch (primitiveType)
			{
				case PT_SPHERE:

					CConfig::ParsePoint(pCurrPrimitiveNode, "center", center);

					pCurrPrimitiveNode->Attribute("radius", &radius);

					if (radius < 0)
					{
						std::cerr << "Error: Incorrect value of radius\n";
						return false;
					}

					AddSphere(center, radius, pCurrMaterial, primitiveColor);

					break;
				case PT_TRIANGLE:

					CConfig::ParsePoint(pCurrPrimitiveNode, "p0", p0);
					CConfig::ParsePoint(pCurrPrimitiveNode, "p1", p1);
					CConfig::ParsePoint(pCurrPrimitiveNode, "p2", p2);

					AddTriangle(p0, p1, p2, pCurrMaterial, primitiveColor);

					break;
				case PT_MESH_TRIANGLE:

					meshFilePath = pCurrPrimitiveNode->Attribute("path");

					if (!LoadMesh(meshFilePath, pCurrMaterial, primitiveColor))
					{
						std::cerr << "Error: Mesh file wasn't found\n";
						return false;
					}

					break;
			}

			pCurrPrimitiveNode = pCurrPrimitiveNode->NextSiblingElement();
		}

		delete pSceneFile;

		return true;
	}

	void CScene::AddSphere(const CPoint& center, const F& radius, const TMaterial* material, const CColor& color)
	{
		mPrimitives.push_back(new CSphere(center, radius, material, color));
		mPrimitivesCount++;
	}

	void CScene::AddTriangle(const CPoint& p0, const CPoint& p1, const CPoint& p2, const TMaterial* material, const CColor& color)
	{
		mPrimitives.push_back(new CTriangle(p0, p1, p2, material, color));
		mPrimitivesCount++;
	}

	bool CScene::LoadMesh(const std::string& filename, const TMaterial* material, const CColor& color)
	{
		COBJLoader* pLoader = new COBJLoader();

		if (!pLoader->Open(filename))
		{
			delete pLoader;
			return false;
		}

		TMeshBuffer* pMeshBuffer = new TMeshBuffer();

		if (!pLoader->ReadMesh(*pMeshBuffer))
			return false;

		pLoader->Close();
		delete pLoader;

		mMeshBuffers.push_back(pMeshBuffer);

		unsigned int indicesCount = pMeshBuffer->mIndices.size();

		for (unsigned int i = 0; i < indicesCount; i += 3)
		{
			mPrimitives.push_back(new CMeshTriangle(mMeshBuffers.back(), pMeshBuffer->mIndices[i], pMeshBuffer->mIndices[i + 1],
				                                    pMeshBuffer->mIndices[i + 2], material, color));
			 
			mPrimitivesCount++;
		}

		return true;
	}

	void CScene::AddPointLight(const CPoint& pos, const CColor& intensity)
	{
		mLightSources.push_back(new CPointLight(pos, intensity));
		mLightSourcesCount++;
	}

	const TMaterial* CScene::AddMaterial(const std::string& name, const F& diffuse, const F& reflection,
		                                 const F& specular, const F& trasnparency, const F& refactCoeff)
	{
		TMaterial* pMaterial = new TMaterial;

		pMaterial->mDiffuse = diffuse;
		pMaterial->mReflection = reflection;
		pMaterial->mSpecular = specular;
		pMaterial->mTransparency = trasnparency;
		pMaterial->mRefractCoeff = refactCoeff;

		mMaterials.insert(std::make_pair(name, pMaterial));

		return pMaterial;
	}

	bool CScene::FindClosestIntersection(const CRay& ray, F& t, THitInfo& info) const
	{
		if (mpBVHTree)
			return mpBVHTree->Hit(ray, t, info);

		ISceneObject* pCurrObject = nullptr;
		F prev = CMath::mInfinityF;
		THitInfo currInfo;

		ISceneObject* pPreCachedObj = nullptr;

		for (unsigned int i = 0; i < mPrimitivesCount; i++)
		{
			pPreCachedObj = mPrimitives[i];

			if (pPreCachedObj->Hit(ray, t, currInfo))
			{
				if (t < prev)
				{
					pCurrObject = pPreCachedObj;
					prev = t;
					info = currInfo;
				}
			}
		}

		t = prev;

		return pCurrObject;
	}

	void CScene::PreCompute(unsigned int bvhMaxDepth)
	{
		mpBVHTree = new CBVHTree();
		mpBVHTree->Build(mPrimitives, bvhMaxDepth);
	}

	void CScene::SetBackgroundColor(const CColor& color)
	{
		mBackgroundColor = color;
	}

	CColor CScene::GetBackgroundColor() const
	{
		return mBackgroundColor;
	}

	const std::vector<ILight*>* CScene::GetLights() const
	{
		return &mLightSources;
	}

	unsigned int CScene::GetLightsCount() const
	{
		return mLightSourcesCount;
	}

	unsigned int CScene::GetPrimitivesCount() const
	{
		return mPrimitivesCount;
	}

	unsigned int CScene::GetBVHTreeMaxDepth() const
	{
		if (mpBVHTree)
			return mpBVHTree->GetMaxDepth();
		else
			return 0;
	}

	const TMaterial* CScene::GetMaterialByName(const std::string& name) const
	{
		std::map<std::string, TMaterial*>::const_iterator iter = mMaterials.find(name);

		if (iter != mMaterials.end())
			return iter->second;
		else
			return nullptr;
	}
}