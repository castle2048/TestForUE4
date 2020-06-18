#pragma once
#define DLL_EXPORT __declspec(dllexport)
#ifdef __cplusplus
extern "C"
{
#endif
	float DLL_EXPORT getCircleArea(float radius);

#ifdef __cplusplus
}
#endif