#ifndef RT_APPLICATION_H
#define RT_APPLICATION_H


#include <string>


namespace RTRender
{

	class CScene;
	class ITracer;
	class ICamera;


	class CApplication
	{
		public:
			CApplication();
			virtual ~CApplication();

			virtual bool Init(const std::string& name, int commandLineArgsCount, char** commandLineArgs);
			virtual void Run();

			std::string GetName() const;
		protected:
			bool mIsReady;

			std::string mName;
		private:
			CScene* mpScene;
			ITracer* mpTracer;
			ICamera* mpCamera;

			unsigned int mMaxDepth;
	};
}

#endif