#ifndef RT_OBJ_LOADER_H
#define RT_OBJ_LOADER_H


#include "interfaces\RTModelLoader.h"
#include <fstream>
#include <vector>


namespace RTRender
{
	class COBJLoader : public IModelLoader
	{
		public:
			COBJLoader();
			virtual ~COBJLoader();

			bool Open(const std::string& filename);
			bool Close();

			bool ReadMesh(TMeshBuffer& mesh);
		private:
			std::ifstream mOBJFile;
	};
}

#endif