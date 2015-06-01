#ifndef RT_LOADER_H
#define RT_LOADER_H


#include <string>
#include <vector>
#include "impls\RTMath.h"


namespace RTRender
{

	struct TMeshBuffer;

	class IModelLoader
	{
		public:
			IModelLoader();
			virtual ~IModelLoader();

			virtual bool Open(const std::string& filename) = 0;
			virtual bool Close() = 0;

			virtual bool ReadMesh(TMeshBuffer& mesh) = 0;
		protected:
			bool mIsOpened;
	};
}

#endif