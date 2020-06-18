#pragma once
//#define LIB_EXPORT __declspec(dllexport)

#ifdef LIB_EXPORT_API
#define LIB_EXPORT __declspec(dllexport)
#else
#define  LIB_EXPORT
#endif

class LIB_EXPORT MyLIB {
public:
	float GetArea(float r);
};