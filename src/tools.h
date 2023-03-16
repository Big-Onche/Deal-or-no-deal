#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <algorithm>
#include <stdarg.h>
#include <unordered_set>
#include <random>
#include <SDL.h>
#include <SDL_image.h>

#ifdef WIN32
  #include <windows.h>
#endif

#define rnd(x) (rand() % x)

#define loop(v,m) for(int v = 0; v < int(m); ++v)
#define loopi(m) loop(i,m)
#define loopj(m) loop(j,m)

#endif
