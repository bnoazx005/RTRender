#include "impls\RTFrameBuffer.h"
#include "impls\RTColor.h"
#include <fstream>
#include "impls\RTTGAImage.h"


namespace RTRender
{

	CFrameBuffer::CFrameBuffer()
	{
	}

	CFrameBuffer::CFrameBuffer(unsigned int width, unsigned int height) :
		mWidth(width), mHeight(height), mpImageBuffer(nullptr)
	{
		mpImageBuffer = new unsigned char[mWidth * mHeight * 3];
	}

	CFrameBuffer::~CFrameBuffer()
	{
		if (mpImageBuffer)
			delete[] mpImageBuffer;
	}

	//void CFrameBuffer::SetPixel(unsigned int x, unsigned int y, const CColor& color)
	//{
	//	if (x >= 0 && y >= 0 && x < mWidth && y < mHeight)
	//		mpImageBuffer[y * mWidth + x] = color.ToR8G8B8Color();
	//}

	//void CFrameBuffer::SaveToFile(const std::string& filename)
	//{
	//	std::ofstream out(filename, std::ios::binary);

	//	char infoStr[255];

	//	/*sprintf(infoStr, "P6\n %d %d\n255\n\0", mWidth, mHeight);
	//	out.write(infoStr, strlen(infoStr));

	//	for (int i = 0; i < mHeight; i++)
	//		for (int j = 0; j < mWidth; j++)
	//			out.write((char*)&mpImageBuffer[i * mWidth + j], 3);

	//	out.close();
	//	IImage* pImage = new CTGAImage(mWidth, mHeight, TIT_TRUE_COLOR_24);
	//	for (int i = 0; i < mHeight; i++)
	//		for (int j = 0; j < mWidth; j++)
	//			pImage->SetPixel(j, i, CColor((F)mpImageBuffer[i * mWidth + j].mR/255, (F)mpImageBuffer[i * mWidth + j].mG/255, (F)mpImageBuffer[i * mWidth + j].mB/255));
	//	pImage->SaveToFile("result_tga.tga");*/
	//	delete pImage;
	//}

	/*const TR8G8B8Color* CFrameBuffer::GetDataPtr() const
	{
		return mpImageBuffer;
	}
*/
	unsigned int CFrameBuffer::GetWidth() const
	{
		return mWidth;
	}

	unsigned int CFrameBuffer::GetHeight() const
	{
		return mHeight;
	}
}