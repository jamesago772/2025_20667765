#pragma once

#ifdef _WIN32
  #ifdef maths_EXPORTS
    #define MATHS_API __declspec(dllexport)
  #else
    #define MATHS_API __declspec(dllimport)
  #endif
#else
  #define MATHS_API
#endif

MATHS_API int add(int a, int b);
