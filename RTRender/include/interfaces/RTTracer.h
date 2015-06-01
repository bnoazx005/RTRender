#ifndef RT_TRACER_H
#define RT_TRACER_H


namespace RTRender
{

	class ICamera;
	class CScene;


	class ITracer
	{
		public:
			ITracer() {}
			virtual ~ITracer() {}

			virtual void Render(ICamera* camera, const CScene* scene, unsigned int maxDepth) = 0;
		protected:
			unsigned int mMaxDepth;
	};
}

#endif