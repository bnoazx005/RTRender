#include <iostream>
#include "RTRender.h"


using namespace std;
using namespace RTRender;


int main(int argc, char** argv)
{
	CApplication* pApplication = new CApplication();

	if (!pApplication->Init("Ray Tracer", argc, argv))
		return -1;

	pApplication->Run();

	delete pApplication;

	return 0;
}