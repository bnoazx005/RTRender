#ifndef RT_SCENE_H
#define RT_SCENE_H


#include <string>
#include <vector>
#include <map>
#include "RTColor.h"
#include "RTMath.h"
#include "RTMesh.h"


namespace RTRender
{
	class ISceneObject;
	class ILight;
	class CBVHTree;

	struct TMaterial;
	struct THitInfo;


	class CScene
	{
		public:
			CScene();
			CScene(const CColor& backgroundColor);
			~CScene();

			bool LoadFromFile(const std::string& filename);

			void AddSphere(const CPoint& center, const F& radius, const TMaterial* material, const CColor& color);
			void AddTriangle(const CPoint& p0, const CPoint& p1, const CPoint& p2, const TMaterial* material, const CColor& color);
			bool LoadMesh(const std::string& filename, const TMaterial* material, const CColor& color);

			void AddPointLight(const CPoint& pos, const CColor& intensity);

			const TMaterial* AddMaterial(const std::string& name,const F& diffuse, const F& reflection,
				                         const F& specular,const F& trasnparency, const F& refactCoeff);

			void PreCompute(unsigned int bvhMaxDepth);

			bool FindClosestIntersection(const CRay& ray, F& t, THitInfo& info) const;

			void SetBackgroundColor(const CColor& color);

			CColor GetBackgroundColor() const;

			const std::vector<ILight*>* GetLights() const;
			unsigned int GetLightsCount() const;
			unsigned int GetPrimitivesCount() const;
			unsigned int GetBVHTreeMaxDepth() const;

			const TMaterial* GetMaterialByName(const std::string& name) const;
		private:
			std::vector<ISceneObject*> mPrimitives;
			unsigned int mPrimitivesCount;

			std::vector<ILight*> mLightSources;
			unsigned int mLightSourcesCount;

			std::map<std::string, TMaterial*> mMaterials;

			CColor mBackgroundColor;

			std::vector<TMeshBuffer*> mMeshBuffers;

			CBVHTree* mpBVHTree;
	};
}

#endif