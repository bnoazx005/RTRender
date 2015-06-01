#include "impls\RTBVHTree.h"
#include "interfaces\RTSceneObject.h"
#include <algorithm>
#include <stack>


namespace RTRender
{

	bool TBVHComparator::operator() (const ISceneObject* object0, const ISceneObject* object1) const
	{
		CPoint center0 = object0->GetBoundingBox().GetCenter();
		CPoint center1 = object1->GetBoundingBox().GetCenter();

		return (center0(mSortAxis) < center1(mSortAxis) - CMath::mEpsilon);
	}


	CBVHTree::CBVHTree() :
		mpRoot(nullptr), mMaxDepth(4)
	{
	}

	CBVHTree::~CBVHTree()
	{
		TBVHNode* pCurrNode = mpRoot;
		
		//deleteRecursive(mpRoot);
	}

	void CBVHTree::Build(const std::vector<ISceneObject*>& objects, unsigned int maxDepth)
	{
		mMaxDepth = maxDepth;

		buildRecursive(&mpRoot, objects, 0);
	}

	void CBVHTree::buildRecursive(TBVHNode** node, const std::vector<ISceneObject*>& objects, unsigned int depth)
	{
		unsigned int objectCount = objects.size();

		if (objectCount == 0)
			return;

		*node = new TBVHNode();
		(*node)->mpLeft = nullptr;
		(*node)->mpRight = nullptr;

		CBoundingBox hull;

		for (unsigned int i = 0; i < objectCount; ++i)
			hull = hull.Union(objects[i]->GetBoundingBox());

		(*node)->mBBox = hull;

		if (objectCount == 1 || depth + 1 == mMaxDepth)
		{
			(*node)->mpObjects = objects;
			return;
		}

		TBVHComparator comparator;
		comparator.mSortAxis = hull.GetIndexOfMaxAxis();

		std::vector<ISceneObject*> objectsCopy(objects.begin(), objects.end());
		std::sort(objectsCopy.begin(), objectsCopy.end(), comparator);

		unsigned int splitIndex = objectCount / 2;

		std::vector<ISceneObject*> leftChildObjects(objectsCopy.begin(), objectsCopy.begin() + splitIndex);
		std::vector<ISceneObject*> rightChildObjects(objectsCopy.begin() + splitIndex, objectsCopy.end());

		//test code

		if (leftChildObjects.size() < 1 && rightChildObjects.size() >= 1)
		{
			delete (*node);
			buildRecursive(&(*node), rightChildObjects, depth + 1);
			return;
		}
		else if (rightChildObjects.size() < 1 && leftChildObjects.size() >= 1)
		{
			delete (*node);
			buildRecursive(&(*node), leftChildObjects, depth + 1);
			return;
		}

		//end of test part of code

		buildRecursive(&(*node)->mpLeft, leftChildObjects, depth + 1);
		buildRecursive(&(*node)->mpRight, rightChildObjects, depth + 1);
	}

	bool CBVHTree::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		TBVHNode* pCurrNode = mpRoot;

		F tmin, tmax;
		
		TBVHNode* pLeftNode = nullptr;
		TBVHNode* pRightNode = nullptr;

		std::stack<TBVHStackItem> nodeStack;
		TBVHStackItem topStackItem;

		ISceneObject* pPreCachedObj = nullptr;
		THitInfo hitResult;
		F prevMin, currMin;
		
		t = CMath::mInfinityF;

		bool leftHit, rightHit;
		F ltmin, ltmax;
		F rtmin, rtmax;

		memset(&info, 0, sizeof(info));

		while (true)
		{

			if (pCurrNode->mBBox.Intersect(ray, tmin, tmax))
			{
				pLeftNode = pCurrNode->mpLeft;
				pRightNode = pCurrNode->mpRight;

				if (pCurrNode->mpObjects.size() > 0)
				{
					pPreCachedObj = nullptr;

					for (unsigned int i = 0; i < pCurrNode->mpObjects.size(); i++)
					{
						pPreCachedObj = pCurrNode->mpObjects[i];

						if (pPreCachedObj->Hit(ray, currMin, hitResult))
						{
							if (currMin < t)
							{
								t = currMin;
								info = hitResult;
							}
						}
					}

					if (nodeStack.empty())
						break;

					topStackItem = nodeStack.top();
					nodeStack.pop();

					pCurrNode = topStackItem.mpNode;
				}
				else
				{
					leftHit = pLeftNode->mBBox.Intersect(ray, ltmin, ltmax);
					rightHit = pRightNode->mBBox.Intersect(ray, rtmin, rtmax);

					if (leftHit && rightHit)
					{
						if (ltmin < rtmin)
						{
							pCurrNode = pLeftNode;

							topStackItem.mpNode = pRightNode;
							topStackItem.mMaxDistance = rtmax;
							topStackItem.mMinDistance = rtmin;

							nodeStack.push(topStackItem);
						}
						else
						{
							pCurrNode = pRightNode;

							topStackItem.mpNode = pLeftNode;
							topStackItem.mMaxDistance = ltmin;
							topStackItem.mMinDistance = ltmax;

							nodeStack.push(topStackItem);
						}
					}
					else if (leftHit)
					{
						pCurrNode = pLeftNode;
					}
					else if (rightHit)
					{
						pCurrNode = pRightNode;
					}
					else
					{
						if (nodeStack.empty())
							break;

						topStackItem = nodeStack.top();
						nodeStack.pop();

						pCurrNode = topStackItem.mpNode;
					}
				}
			}
			else
			{
				if (nodeStack.empty())
					break;

				topStackItem = nodeStack.top();
				nodeStack.pop();

				pCurrNode = topStackItem.mpNode;
			}
		}

		return info.mpObject != nullptr;
	}

	void CBVHTree::deleteRecursive(TBVHNode* &node)
	{
		if (node->mpLeft == nullptr && node->mpRight == nullptr)
		{
			delete node;
			node = nullptr;

			return;
		}

		deleteRecursive(node->mpLeft);
		deleteRecursive(node->mpRight);

		delete node;
		node = nullptr;
	}

	unsigned int CBVHTree::GetMaxDepth() const
	{
		return mMaxDepth;
	}
}