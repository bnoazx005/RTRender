#ifndef RT_RAY_TRACER_H
#define RT_RAY_TRACER_H


#include "interfaces\RTTracer.h"
#include "RTConfig.h"
#include "RTTypes.h"


namespace RTRender
{
	class CRay;
	class CColor;

	struct THitInfo;

	
	enum E_TRACER_TYPE
	{
		TT_MPI,
		TT_OMP,
		TT_SEQUENTIAL
	};


	class CRayTracer : public ITracer
	{
		public:
			CRayTracer();
			virtual ~CRayTracer();

			virtual void Render(ICamera* camera, const CScene* scene, unsigned int maxDepth);
		protected:
			CColor traceRay(const CRay& ray, unsigned int depth);

			CColor computeShading(const THitInfo& info, const F& t);
			CColor computeReflection(const THitInfo& info, unsigned int depth);
			CColor computeRefraction(const THitInfo& info, unsigned int depth);
		protected:
			const CScene* mpScene;
	};
}

#endif