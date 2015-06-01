#ifndef RT_BVH_TREE_H
#define RT_BVH_TREE_H


#include "impls\RTBoundingBox.h"
#include <vector>


namespace RTRender
{

	class ISceneObject;

	struct THitInfo;


	#pragma pack(push, 1)

	struct TBVHNode
	{
		TBVHNode* mpLeft;
		TBVHNode* mpRight;

		CBoundingBox mBBox;

		std::vector<ISceneObject*> mpObjects;
	};


	struct TBVHComparator
	{
		bool operator() (const ISceneObject* object0, const ISceneObject* object1) const;

		unsigned char mSortAxis;
	};

	struct TBVHStackItem
	{
		TBVHNode* mpNode;
		F mMinDistance;
		F mMaxDistance;
	};

	#pragma pack(pop)


	class CBVHTree
	{
		public:
			CBVHTree();
			~CBVHTree();

			void Build(const std::vector<ISceneObject*>& objects, unsigned int maxDepth);
			bool Hit(const CRay& ray, F& t, THitInfo& info) const;

			unsigned int GetMaxDepth() const;
		private:
			void buildRecursive(TBVHNode** node, const std::vector<ISceneObject*>& objects, unsigned int depth);

			void deleteRecursive(TBVHNode* &node);
		private:
			TBVHNode* mpRoot;

			unsigned int mMaxDepth;
	};
}

#endif