#include "impls\RTLog.h"
#include <stdarg.h>


namespace RTRender
{

	CLog* CLog::mpLogInst = nullptr;


	CLog::CLog(const std::string& filename) :
		mFilename(filename)
	{
		mOutFile.open(filename.c_str(),std::ios::app);

		if (!mOutFile.is_open())
			mpLogInst = nullptr;
		else
			mOutFile << filename << std::endl;

		mOutFile.close();
	}

	CLog::~CLog()
	{
		if (mOutFile.is_open())
			mOutFile.close();
	}

	CLog* CLog::GetInst(const std::string& filename)
	{
		if (!mpLogInst)
			mpLogInst = new CLog(filename);

		return mpLogInst;
	}

	void CLog::Write(const std::string& message)
	{
		mOutFile.open(mFilename.c_str(), std::ios::app);

		mOutFile << ">> " << message << std::endl;

		mOutFile.close();
	}

	void CLog::Write(const char* params, ...)
	{
		char buf[255];

		va_list mark;
		va_start(mark, params);

		int size = _vsnprintf(buf, 256, params, mark);
		buf[254] = 0;

		mOutFile.open(mFilename.c_str(), std::ios::app);

		if (!mOutFile.is_open())
			return;

		mOutFile << ">> " << buf << std::endl;
		mOutFile.close();
	}
}