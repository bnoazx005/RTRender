#include "impls\RTOBJLoader.h"
#include <fstream>
#include "impls\RTMesh.h"


namespace RTRender
{

	COBJLoader::COBJLoader():
		IModelLoader()
	{
	}

	COBJLoader::~COBJLoader()
	{
	}

	bool COBJLoader::Open(const std::string& filename)
	{
		if (mIsOpened)
			return false;

		mOBJFile.open(filename.c_str());

		if (!mOBJFile.is_open())
			return false;

		mIsOpened = true;

		return true;
	}

	bool COBJLoader::Close()
	{
		if (!mIsOpened)
			return false;

		if (mOBJFile.is_open())
			mOBJFile.close();

		return true;
	}

	bool COBJLoader::ReadMesh(TMeshBuffer& mesh)
	{
		if (!mIsOpened)
			return false;

		std::string currStr, currValue;
		CPoint tmpVector;
		CNormal tmpNormal;
		unsigned int tmpIndex;
		unsigned int tmpFaceIndex;

		mesh.mNumIndices = 0;
		mesh.mNumVertices = 0;

		do
		{
			mOBJFile >> currStr;

			if (currStr == "v")
			{
				mOBJFile >> currStr;
				tmpVector.x = atof(currStr.c_str());

				mOBJFile >> currStr;
				tmpVector.y = atof(currStr.c_str());

				mOBJFile >> currStr;
				tmpVector.z = atof(currStr.c_str());

				mesh.mVertices.push_back(tmpVector);
				mesh.mNumVertices++;
			}
			else if (currStr == "f")
			{
				mOBJFile >> currStr;

				if ((tmpIndex = currStr.find_first_of('/')) >= 0)
				{
					currValue = currStr.substr(0, tmpIndex);
					tmpFaceIndex = atoi(currValue.c_str());
					mesh.mIndices.push_back(tmpFaceIndex - 1);

					mOBJFile >> currStr;
					tmpIndex = currStr.find_first_of('/');
					currValue = currStr.substr(0, tmpIndex);
					tmpFaceIndex = atoi(currValue.c_str());
					mesh.mIndices.push_back(tmpFaceIndex - 1);

					mOBJFile >> currStr;
					tmpIndex = currStr.find_first_of('/');
					currValue = currStr.substr(0, tmpIndex);
					tmpFaceIndex = atoi(currValue.c_str());
					mesh.mIndices.push_back(tmpFaceIndex - 1);
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						tmpFaceIndex = atoi(currStr.c_str());
						mesh.mIndices.push_back(tmpFaceIndex - 1);

						mOBJFile >> currStr;
					}
				}

				mesh.mNumIndices += 3;
			}
			else if (currStr == "vn")
			{
				mOBJFile >> currStr;
				tmpNormal.x = atof(currStr.c_str());

				mOBJFile >> currStr;
				tmpNormal.y = atof(currStr.c_str());

				mOBJFile >> currStr;
				tmpNormal.z = atof(currStr.c_str());

				mesh.mNormals.push_back(tmpNormal);
			}
		} while (!mOBJFile.eof());

		return true;
	}
}