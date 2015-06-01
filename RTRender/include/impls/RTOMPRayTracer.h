#ifndef RT_OMP_RAY_TRACER_H
#define RT_OMP_RAY_TRACER_H


#include "impls\RTRayTracer.h"


namespace RTRender
{

	class COMPRayTracer : public CRayTracer
	{
	public:
		COMPRayTracer(unsigned int schemeId);
		~COMPRayTracer();

		virtual void Render(ICamera* camera, const CScene* scene, unsigned int maxDepth);
	private:
		COMPRayTracer();

		void renderScheme1(ICamera* camera, const CScene* scene, unsigned int maxDepth);
		void renderScheme2(ICamera* camera, const CScene* scene, unsigned int maxDepth);
	private:
		unsigned int mSchemeId;
	};
}

#endif