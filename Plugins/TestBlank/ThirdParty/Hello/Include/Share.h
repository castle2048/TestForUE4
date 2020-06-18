#pragma once
#ifdef HELLO_API_EXPORTS
#define HELLO_API __declspec(dllexport)
#elif defined(HELLP_API_IMPORTS)
#define HELLO_API __declspec(dllimport)
#else
#define  HELLO_API
#endif

//#define  HELLO_API __declspec(dllexport)