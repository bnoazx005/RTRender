#include "impls\RTOMPRayTracer.h"
#include "impls\RTColor.h"
#include "impls\RTTGAImage.h"
#include "interfaces\RTCamera.h"
#include "interfaces\RTSceneObject.h"
#include "interfaces\RTLight.h"
#include "impls\RTScene.h"
#include "impls\RTMath.h"
#include <vector>
#include <iostream>
#include "impls\RTLog.h"
#include <omp.h>


namespace RTRender
{

	COMPRayTracer::COMPRayTracer() :
		CRayTracer()
	{
	}

	COMPRayTracer::COMPRayTracer(unsigned int schemeId) :
		CRayTracer(), mSchemeId(schemeId)
	{
	}

	COMPRayTracer::~COMPRayTracer()
	{
	}

	void COMPRayTracer::Render(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (mSchemeId == 1)
			renderScheme1(camera, scene, maxDepth);
		else if (mSchemeId == 2)
			renderScheme2(camera, scene, maxDepth);
		else
		{
			std::cerr << "Incorrect scheme's index\n";
			return;
		}
	}

	void COMPRayTracer::renderScheme1(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (!camera || !scene)
			return;

		IImage* pImageBuffer = camera->GetImageBuffer();
		mpScene = scene;

		unsigned int width = pImageBuffer->GetWidth();
		unsigned int height = pImageBuffer->GetHeight();

		CRay ray;
		CColor finalColor;

		unsigned int chunkSize = pImageBuffer->GetWidth() / 8;

		int x, y;

		double timeOfStart = omp_get_wtime();

		for (y = 0; y < height; ++y)
		{
			#pragma omp parallel for private(x, ray, finalColor) schedule(dynamic, chunkSize)
			for (x = 0; x < width; ++x)
			{
				ray = camera->ComputeRay(x, y);

				finalColor = traceRay(ray, 0).ToRange();

				pImageBuffer->SetPixel(x, y, finalColor);
			}
		}

		double timeOfFinish = omp_get_wtime();
		double elapsedTime = timeOfFinish - timeOfStart;

		CLog* pLog = CLog::GetInst("results.log");

		pLog->Write("Elapsed time(OpenMP parallelism, scheme #%u, BVH max depth: %u) : %.4f",
			        mSchemeId, scene->GetBVHTreeMaxDepth(), elapsedTime);

		pImageBuffer->SaveToFile(pImageBuffer->GetFilename());
	}

	void COMPRayTracer::renderScheme2(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (!camera || !scene)
			return;

		IImage* pImageBuffer = camera->GetImageBuffer();
		mpScene = scene;

		unsigned int width = pImageBuffer->GetWidth();
		unsigned int height = pImageBuffer->GetHeight();

		CRay ray;
		CColor finalColor;

		unsigned int chunkSize = pImageBuffer->GetHeight() / 8;

		int x, y;

		double timeOfStart = omp_get_wtime();

		#pragma omp parallel for private(x,y, ray, finalColor) schedule(dynamic, chunkSize)
		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				ray = camera->ComputeRay(x, y);

				finalColor = traceRay(ray, 0).ToRange();

				pImageBuffer->SetPixel(x, y, finalColor);
			}
		}

		double timeOfFinish = omp_get_wtime();
		double elapsedTime = timeOfFinish - timeOfStart;

		CLog* pLog = CLog::GetInst("results.log");

		pLog->Write("Elapsed time(OpenMP parallelism, scheme #%u, BVH max depth: %u) : %.4f",
			        mSchemeId, scene->GetBVHTreeMaxDepth(), elapsedTime);

		pImageBuffer->SaveToFile(pImageBuffer->GetFilename());
	}
}