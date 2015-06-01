#include "impls\RTRayTracer.h"
#include "impls\RTColor.h"
#include "impls\RTTGAImage.h"
#include "interfaces\RTCamera.h"
#include "interfaces\RTSceneObject.h"
#include "interfaces\RTLight.h"
#include "impls\RTScene.h"
#include "impls\RTMath.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include "impls\RTLog.h"


namespace RTRender
{

	CRayTracer::CRayTracer() :
		ITracer()
	{
	}

	CRayTracer::~CRayTracer()
	{
	}

	void CRayTracer::Render(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (!camera || !scene)
			return;

		IImage* pImageBuffer = camera->GetImageBuffer();
		mpScene = scene;

		unsigned int width = pImageBuffer->GetWidth();
		unsigned int height = pImageBuffer->GetHeight();

		CRay ray;
		CColor finalColor;

		double timeOfStart = omp_get_wtime();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				ray = camera->ComputeRay(x, y);

				finalColor = traceRay(ray, 0).ToRange();

				pImageBuffer->SetPixel(x, y, finalColor);
			}
		}

		double timeOfFinish = omp_get_wtime();
		double elapsedTime = timeOfFinish - timeOfStart;

		CLog* pLog = CLog::GetInst("results.log");
		pLog->Write("Elapsed time(sequential, BVH max depth: %u) : %.4f", scene->GetBVHTreeMaxDepth(), elapsedTime);
		
		pImageBuffer->SaveToFile(pImageBuffer->GetFilename());
	}

	CColor CRayTracer::traceRay(const CRay& ray, unsigned int depth)
	{
		CColor finalColor;
		THitInfo hitInfo, shadowHitInfo;
		F t;

		if (!mpScene->FindClosestIntersection(ray, t, hitInfo))
			return mpScene->GetBackgroundColor();

		finalColor = computeShading(hitInfo, t);

		const TMaterial* pMaterial = hitInfo.mpMaterial;

		if (pMaterial->mReflection > 0.0 && depth <= mMaxDepth)
			finalColor += computeReflection(hitInfo, depth + 1);

		if (pMaterial->mTransparency > 0.0 && depth <= mMaxDepth)
			finalColor += computeRefraction(hitInfo, depth + 1);

		return finalColor;
	}

	CColor CRayTracer::computeShading(const THitInfo& info, const F& t)
	{
		CColor color;

		THitInfo shadowHitInfo;
		CRay shadowRay;

		F st;
		F distToLight = 0.0;

		unsigned int lightsCount = mpScene->GetLightsCount();
		const std::vector<ILight*>* pLights = mpScene->GetLights();

		const ILight* pCurrLight = nullptr;
		
		CPoint hitPoint = info.mPoint;
		CColor hitColor = info.mColor;
		CNormal hitNormal = info.mNormal;

		const TMaterial* pMaterial = info.mpMaterial;

		for (unsigned int i = 0; i < lightsCount; ++i)
		{
			pCurrLight = (*pLights)[i];

			shadowRay.origin = hitPoint;
			shadowRay.dir = pCurrLight->GetPos() - hitPoint;
			distToLight = shadowRay.dir.Length();
			shadowRay.dir.Normalize();

			if (mpScene->FindClosestIntersection(shadowRay, st, shadowHitInfo) && (st <= distToLight))
				continue;
			
			//Lambert
			if (pMaterial->mDiffuse > 0.0)
			{
				F lambert = CMath::Maxf(shadowRay.dir.DotProduct(hitNormal), 0.0);
				color += (hitColor * pCurrLight->GetIntensity()) * (lambert * pMaterial->mDiffuse);
			}

			//Phong
			if (pMaterial->mSpecular > 0.0)
			{
				CRay reflectedRay(hitPoint, shadowRay.dir - hitNormal * (2.0 * shadowRay.dir.DotProduct(hitNormal)));
				reflectedRay.dir.Normalize();

				F dot = CMath::Maxf(reflectedRay.dir.DotProduct(info.mRay.dir), 0.0);
				F spec = powf(dot, 20.0) * pMaterial->mSpecular;

				color += (color * pCurrLight->GetIntensity()) * spec;
			}
		}

		return color;
	}

	CColor CRayTracer::computeReflection(const THitInfo& info, unsigned int depth)
	{
		CNormal hitNormal(info.mNormal);
		CRay viewRay(info.mRay);

		CRay reflectedRay(info.mPoint, viewRay.dir - hitNormal * (2.0 * viewRay.dir.DotProduct(hitNormal)));
		reflectedRay.dir.Normalize();

		return traceRay(reflectedRay, depth + 1) * info.mpMaterial->mReflection;
	}

	CColor CRayTracer::computeRefraction(const THitInfo& info, unsigned int depth)
	{
		CNormal hitNormal(info.mNormal);
		CPoint hitPoint(info.mPoint);
		CRay ray(info.mRay);

		F eta = 1.0 / info.mpMaterial->mRefractCoeff;

		F cosTheta = -ray.dir.DotProduct(hitNormal);

		if (cosTheta < CMath::mEpsilon)
		{
			cosTheta *= -1.0;
			hitNormal = hitNormal * -1.0;
			eta = 1.0 / eta;
		}

		F tir = 1.0 - eta * eta * (1.0 - cosTheta * cosTheta);

		CRay refractedRay;

		if (tir <= 1.0)
		{
			refractedRay.dir = ray.dir * eta + hitNormal * (eta * cosTheta - sqrt(tir));
			refractedRay.dir.Normalize();
			refractedRay.origin = hitPoint + refractedRay.dir * CMath::mEpsilon;

			return traceRay(refractedRay, depth + 1) * info.mpMaterial->mTransparency;
		}
	}
}