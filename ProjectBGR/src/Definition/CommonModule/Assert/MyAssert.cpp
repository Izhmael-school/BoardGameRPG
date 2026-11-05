#include "MyAssert.h"
#include <Windows.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "../Json/MyJson.h"

#ifdef _DEBUG
#define MY_ASSERT( msg)                                              \
    do {                                                                  \
            char buf[512];                                                \
            snprintf(buf, sizeof(buf), "%s\n\n%s(%d)", (msg), __FILE__, __LINE__); \
            MessageBoxW(NULL,MyJson::Utf8ToWString(buf).c_str(), L"Assertion Failed", MB_OK | MB_ICONERROR); \
    } while (0)
#else
#define MY_ASSERT( msg) ((void)0)
#endif

void MyAssert::Assert(const std::string& _text) {
	MY_ASSERT(_text.c_str());
}

void MyAssert::DebugAssert(const std::string& _text) {
#if _DEBUG
	Assert(_text);
#endif
}
