#ifndef RT_MESH_H
#define RT_MESH_H


#include "RTMath.h"
#include "interfaces\RTModelLoader.h"
#include "interfaces\RTSceneObject.h"


namespace RTRender
{

	struct TMeshBuffer
	{
			std::vector<CPoint> mVertices;
			std::vector<CNormal> mNormals;
			std::vector<unsigned int> mIndices;

			unsigned int mNumVertices;
			unsigned int mNumIndices;
	};

	class CMeshTriangle : public ISceneObject
	{
		public:
			CMeshTriangle(const TMeshBuffer* mesh, const unsigned int& i0, const unsigned int& i1, const unsigned int& i2,
				          const TMaterial* material, const CColor& color);

			virtual ~CMeshTriangle();

			virtual bool Hit(const CRay& ray, F& t, THitInfo& info) const; 
			bool IsInsideBBox(const CBoundingBox& bbox) const;

			CBoundingBox GetBoundingBox() const;
		protected:
			const TMeshBuffer* mpMesh;
			unsigned int mI0, mI1, mI2;
			CNormal mNormal;
	};

	class CSmoothMeshTriangle : public CMeshTriangle
	{
		public:
			CSmoothMeshTriangle(const TMeshBuffer* mesh, const unsigned int& i0, const unsigned int& i1, const unsigned int& i2,
				const TMaterial* material, const CColor& color);

			virtual ~CSmoothMeshTriangle();

			bool Hit(const CRay& ray, F& t, THitInfo& info) const;
		protected:
			CNormal interpolateNormal(const F& b, const F& g) const;
	};
}

#endif