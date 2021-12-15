#include "..\..\Header\Util\Utility.h"
#include <stdio.h>
#include <Windows.h>

void GatesEngine::Utility::Printf(const char* format, ...)
{
	// Debug‚Ì‚İÀs
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif
}
