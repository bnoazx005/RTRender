//#include "impls\RTKDTree.h"
//#include "impls\RTMath.h"
//
//
//namespace RTRender
//{
//
//	CKDTree::CKDTree(const unsigned int& maxTreeDepth) :
//		mMaxTreeDepth(maxTreeDepth)
//	{
//	}
//
//	CKDTree::~CKDTree()
//	{
//	}
//
//	bool CKDTree::Build(const std::vector<ISceneObject*>& objects, unsigned int depth)
//	{
//		return buildNode(mpRoot, objects, depth); 
//	}
//
//	bool CKDTree::Hit(const CRay& ray, F& tmin, F& tmax, THitInfo& info) const
//	{
//		if (!mpRoot)
//			return false;
//
//		return hitNode(mpRoot, ray, tmin, tmax, info);
//	}
//
//	bool CKDTree::buildNode(TKDNode*& node, const std::vector<ISceneObject*>& objects, unsigned int depth)
//	{
//		if (depth >= mMaxTreeDepth || objects.size() < 1)
//			return false;
//
//		node = new TKDNode();
//
//		node->mpLeft = nullptr;
//		node->mpRight = nullptr;
//		
//		if(depth == mMaxTreeDepth - 1)
//			node->mpObjects = objects;
//
//		if (objects.size() == 0)
//			return false;
//		
//		//contruct the bbox of whole scene
//		node->mBBox = objects[0]->GetBoundingBox();
//
//		for (unsigned int i = 1; i < objects.size(); i++)
//			node->mBBox = node->mBBox.Union(objects[i]->GetBoundingBox());
//		
//		//split bbox
//		CBoundingBox preCachedBox = node->mBBox;
//
//		unsigned char axisIndex = preCachedBox.GetIndexOfMaxAxis();
//		CPoint bboxCenter = preCachedBox.GetCenter();
//
//		std::vector<ISceneObject*> leftObjects;
//		std::vector<ISceneObject*> rightObjects;
//		
//		CBoundingBox leftBox, rightBox;
//
//		switch (axisIndex)
//		{
//			case 0:
//				leftBox.min = preCachedBox.min;
//				leftBox.max = CPoint(bboxCenter.x, preCachedBox.max.y, preCachedBox.max.z);
//
//				rightBox.min = CPoint(bboxCenter.x, preCachedBox.min.y, preCachedBox.min.z);
//				rightBox.max = preCachedBox.max;
//				break;
//			case 1:		
//				leftBox.min = preCachedBox.min;
//				leftBox.max = CPoint(preCachedBox.max.x, bboxCenter.y, preCachedBox.max.z);
//
//				rightBox.min = CPoint(preCachedBox.min.x, bboxCenter.y, preCachedBox.min.z);
//				rightBox.max = preCachedBox.max;
//				break;
//			case 2:
//				leftBox.min = preCachedBox.min;
//				leftBox.max = CPoint(preCachedBox.max.x, preCachedBox.max.y, bboxCenter.z);
//
//				rightBox.min = CPoint(preCachedBox.min.x, preCachedBox.min.y, bboxCenter.z);
//				rightBox.max = preCachedBox.max;
//				break;
//		}
//
//		for (int i = 0; i < objects.size(); i++)
//		{
//			if (objects[i]->IsInsideBBox(leftBox))
//				leftObjects.push_back(objects[i]);
//			else
//				rightObjects.push_back(objects[i]);
//		}
//
//		buildNode(node->mpLeft, leftObjects, depth + 1);
//		buildNode(node->mpRight, rightObjects, depth + 1);
//
//		return true;
//	}
//
//	bool CKDTree::hitNode(const TKDNode* currNode, const CRay& ray, F& tmin, F& tmax, THitInfo& info) const
//	{
//		if (!currNode)
//			return false;
//
//		if (!currNode->mBBox.Intersect(ray))
//			return false;
//
//		TKDNode* pLeft = currNode->mpLeft;
//		TKDNode* pRight = currNode->mpRight;
//
//		if (!pLeft && !pRight)
//		{
//			ISceneObject* pCurrObject = nullptr;
//			F prev = CMath::mInfinityF;
//			THitInfo currInfo;
//
//			ISceneObject* pPreCachedObj = nullptr;
//
//			for (int i = 0; i < currNode->mpObjects.size(); i++)
//			{
//				pPreCachedObj = currNode->mpObjects[i];
//
//				if (pPreCachedObj->Hit(ray, tmin, currInfo))
//				{
//					if (tmin < prev)
//					{
//						pCurrObject = pPreCachedObj;
//						prev = tmin;
//						info = currInfo;
//					}
//				}
//			}
//
//			tmin = prev;
//		}
//
//		bool isLeftIntersection = false;
//		bool isRightIntersection = false;
//
//		if ((isLeftIntersection = pLeft->mBBox.Intersect(ray)) && (isRightIntersection = pRight->mBBox.Intersect(ray)))
//		{
//			return hitNode(pLeft, ray, tmin, tmax, info) && hitNode(pRight, ray, tmin, tmax, info);
//		}
//		else if (isLeftIntersection)
//			return hitNode(pLeft, ray, tmin, tmax, info);
//		else
//			return hitNode(pRight, ray, tmin, tmax, info);
//
//		return false;
//	}
//
//}