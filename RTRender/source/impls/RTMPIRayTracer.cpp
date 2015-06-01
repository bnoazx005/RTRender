#include "impls\RTMPIRayTracer.h"
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
#include <mpi.h>


#pragma comment(lib, "msmpi")


namespace RTRender
{

	CMPIRayTracer::CMPIRayTracer():
		CRayTracer()
	{
	}

	CMPIRayTracer::CMPIRayTracer(int commandLineArgsCount, char** commandLineArgs, unsigned int schemeId) :
		CRayTracer(), mCommandLineParams(commandLineArgs), mCommandLineParamsCount(commandLineArgsCount), mSchemeId(schemeId)
	{
	}

	CMPIRayTracer::~CMPIRayTracer()
	{
	}

	void CMPIRayTracer::Render(ICamera* camera, const CScene* scene, unsigned int maxDepth)
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

	void CMPIRayTracer::renderScheme1(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (!camera || !scene)
			return;

		IImage* pImageBuffer = camera->GetImageBuffer();
		mpScene = scene;

		unsigned int width = pImageBuffer->GetWidth();
		unsigned int height = pImageBuffer->GetHeight();

		CRay ray;
		CColor finalColor;

		MPI_Init(&mCommandLineParamsCount, &mCommandLineParams);

		int processId = 0;
		int processCount = 0;

		MPI_Comm_rank(MPI_COMM_WORLD, &processId);
		MPI_Comm_size(MPI_COMM_WORLD, &processCount);

		unsigned int totalSize = width * height;
		unsigned int size = totalSize / processCount;
		unsigned int bufIdx = 0;
		unsigned int x, y;
		unsigned int upperBorder = 0;
		unsigned char* pBuffer = nullptr;
		unsigned char* pResultBuffer = new unsigned char[totalSize * 3];

		int* pBlocksLengths = new int[processCount];
		int* pBlocksOffsets = new int[processCount];

		for (int i = 0; i < processCount; i++)
		{
			pBlocksLengths[i] = size * 3;
			pBlocksOffsets[i] = i * size * 3;
		}

		pBlocksLengths[processCount - 1] = (totalSize + (1 - processCount) * size) * 3;

		pBuffer = new unsigned char[pBlocksLengths[processId] * 3];

		F timeOfStart = MPI_Wtime();

		if (totalSize - size * processCount > 0)
		{
			upperBorder = (pBlocksOffsets[processId] + pBlocksLengths[processId]) / 3;

			for (unsigned int i = pBlocksOffsets[processId] / 3; i < upperBorder; i++)
			{
				x = i % width;
				y = int(i / width);

				ray = camera->ComputeRay(x, y);

				finalColor = traceRay(ray, 0).ToRange();

				pBuffer[bufIdx] = unsigned char(finalColor.r * 255);
				pBuffer[bufIdx + 1] = unsigned char(finalColor.g * 255);
				pBuffer[bufIdx + 2] = unsigned char(finalColor.b * 255);

				bufIdx += 3;
			}

			MPI_Gatherv(pBuffer, pBlocksLengths[processId], MPI_UNSIGNED_CHAR, pResultBuffer, pBlocksLengths,
				pBlocksOffsets, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
		}
		else
		{
			for (unsigned int i = size * processId; i < (processId + 1) * size; i++)
			{
				x = i % width;
				y = int(i / width);

				ray = camera->ComputeRay(x, y);

				finalColor = traceRay(ray, 0).ToRange();

				pBuffer[bufIdx] = unsigned char(finalColor.r * 255);
				pBuffer[bufIdx + 1] = unsigned char(finalColor.g * 255);
				pBuffer[bufIdx + 2] = unsigned char(finalColor.b * 255);

				bufIdx += 3;
			}

			MPI_Gather(pBuffer, size * 3, MPI_UNSIGNED_CHAR, pResultBuffer, size * 3, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
		}

		F timeOfFinish = MPI_Wtime();

		delete[] pBuffer;
		delete[] pBlocksLengths;
		delete[] pBlocksOffsets;

		if (processId == 0)
		{
			F elapsedTime = timeOfFinish - timeOfStart;

			CLog* pLog = CLog::GetInst("results.log");

			pLog->Write("Elapsed time(MPI parallelism, scheme #%u, BVH max depth: %u) : %.4f",
				        mSchemeId, scene->GetBVHTreeMaxDepth(), elapsedTime);

			pImageBuffer->Copy(pResultBuffer, totalSize * 3);
			pImageBuffer->SaveToFile(pImageBuffer->GetFilename());

			/*std::ofstream out("result.ppm", std::ios::binary);

			char infoStr[255];

			sprintf(infoStr, "P6\n %d %d\n255\n\0", width, height);
			out.write(infoStr, strlen(infoStr));

			for (unsigned int i = 0; i < totalSize * 3; i++)
				out.write((char*)&pResultBuffer[i], 1);

			out.close();*/

			delete[] pResultBuffer;
		}

		MPI_Finalize();
	}

	void CMPIRayTracer::renderScheme2(ICamera* camera, const CScene* scene, unsigned int maxDepth)
	{
		if (!camera || !scene)
			return;

		IImage* pImageBuffer = camera->GetImageBuffer();
		mpScene = scene;

		unsigned int width = pImageBuffer->GetWidth();
		unsigned int height = pImageBuffer->GetHeight();

		CRay ray;
		CColor finalColor;

		MPI_Init(&mCommandLineParamsCount, &mCommandLineParams);

		int processId = 0;
		int processCount = 0;

		MPI_Comm_rank(MPI_COMM_WORLD, &processId);
		MPI_Comm_size(MPI_COMM_WORLD, &processCount);

		/*unsigned int totalSize = width * height;
		unsigned int size = totalSize / processCount;
		unsigned int x, y;

		TR8G8B8Color* pBuffer = new TR8G8B8Color[size];
		TR8G8B8Color* pResultBuffer = new TR8G8B8Color[totalSize];

		int bufIdx = 0;

		MPI_Datatype colorType;

		int len[4] = { 1, 1, 1, 1 };
		MPI_Aint pos[4] = { offsetof(TR8G8B8Color, mR), offsetof(TR8G8B8Color, mG), offsetof(TR8G8B8Color, mB), sizeof(TR8G8B8Color) };
		MPI_Datatype typ[4] = { MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, MPI_UB };

		MPI_Type_struct(4, len, pos, typ, &colorType);
		MPI_Type_commit(&colorType);*/

		unsigned int totalSize = width * height * 3;
		unsigned int stride = 3 * width;
		int nStrides = height / processCount;
		unsigned int sendCount = 0;
		unsigned char* pSendBuffer = new unsigned char[stride];
		unsigned char* pRecvBuffer = nullptr;
		unsigned char* pResultBuffer = nullptr;

		if (processId == 0)
			pResultBuffer = new unsigned char[totalSize];

		unsigned int x, y;
		unsigned int bufIdx = 0;

		MPI_Status status;

		F timeOfStart = MPI_Wtime();

		if (processId == 0)
		{
			for (unsigned int i = processId; i < height; i += processCount)
			{
				y = i;
				bufIdx = 0;

				for (x = 0; x < width; x++)
				{
					ray = camera->ComputeRay(x, y);

					finalColor = traceRay(ray, 0).ToRange();

					pSendBuffer[bufIdx] = unsigned char(finalColor.r * 255);
					pSendBuffer[bufIdx + 1] = unsigned char(finalColor.g * 255);
					pSendBuffer[bufIdx + 2] = unsigned char(finalColor.b * 255);

					bufIdx += 3;
				}

				memcpy(&pResultBuffer[i*stride], pSendBuffer, stride);
			}

			int count = 0;
			int idx = 0;

			for (unsigned int i = 1; i < processCount; i++)
			{
				MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
				MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &count);

				pRecvBuffer = new unsigned char[count];

				MPI_Recv(pRecvBuffer, count, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

				count /= stride;
				idx = status.MPI_SOURCE;

				for (unsigned int j = 0; j < count; j++)
				{
					memcpy(&pResultBuffer[idx * stride], &pRecvBuffer[j * stride], stride);
					idx += processCount;
				}
			}
		}
		else
		{
			std::vector<unsigned char> sendBuffer;

			for (unsigned int i = processId; i < height; i += processCount)
			{
				y = i;
				bufIdx = 0;

				for (x = 0; x < width; x++)
				{
					ray = camera->ComputeRay(x, y);

					finalColor = traceRay(ray, 0).ToRange();

					sendBuffer.push_back(unsigned char(finalColor.r * 255));
					sendBuffer.push_back(unsigned char(finalColor.g * 255));
					sendBuffer.push_back(unsigned char(finalColor.b * 255));
				}
			}

			MPI_Send(&sendBuffer[0], sendBuffer.size(), MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD);
		}

		F timeOfFinish = MPI_Wtime();

		delete[] pSendBuffer;
		delete[] pRecvBuffer;

		if (processId == 0)
		{
			F elapsedTime = timeOfFinish - timeOfStart;

			CLog* pLog = CLog::GetInst("results.log");

			pLog->Write("Elapsed time(MPI parallelism, scheme #%u, BVH max depth: %u) : %.4f",
				        mSchemeId,  scene->GetBVHTreeMaxDepth(), elapsedTime);

			pImageBuffer->Copy(pResultBuffer, totalSize);
			pImageBuffer->SaveToFile(pImageBuffer->GetFilename());

			/*std::ofstream out("result.ppm", std::ios::binary);

			char infoStr[255];

			sprintf(infoStr, "P6\n %d %d\n255\n\0", width, height);
			out.write(infoStr, strlen(infoStr));

			for (unsigned int i = 0; i < totalSize; i++)
				out.write((char*)&pResultBuffer[i], 1);

			out.close();*/

			delete[] pResultBuffer;
		}

		MPI_Finalize();
	}
}