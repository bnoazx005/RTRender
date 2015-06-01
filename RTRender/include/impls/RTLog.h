#ifndef RT_LOG_H
#define RT_LOG_H


#include <string>
#include <fstream>


namespace RTRender
{

	class CLog
	{
		public:
			~CLog();

			static CLog* GetInst(const std::string& filename);
			void Write(const std::string& message);
			void Write(const char* params, ...);
		private:
			CLog(const std::string& filename);
		private:
			static CLog* mpLogInst;

			std::string mFilename;
			std::ofstream mOutFile;
	};
}

#endif