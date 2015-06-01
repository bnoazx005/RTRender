#ifndef RT_MPI_RAY_TRACER_H
#define RT_MPI_RAY_TRACER_H


#include "impls\RTRayTracer.h"


namespace RTRender
{

	class CMPIRayTracer : public CRayTracer
	{
		public:
			CMPIRayTracer(int commandLineArgsCount, char** commandLineArgs, unsigned int schemeId);
			~CMPIRayTracer();

			virtual void Render(ICamera* camera, const CScene* scene, unsigned int maxDepth);
		private:
			CMPIRayTracer();

			void renderScheme1(ICamera* camera, const CScene* scene, unsigned int maxDepth);
			void renderScheme2(ICamera* camera, const CScene* scene, unsigned int maxDepth);
		private:
			unsigned int mSchemeId;

			int mCommandLineParamsCount;
			char** mCommandLineParams;
	};
}

#endif