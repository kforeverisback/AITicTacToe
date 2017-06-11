#pragma once
#if !defined(__GNUC__) && defined(_DEBUG)
//For Memory Leak Detection
#include <vld.h>
#endif

//C++ Headers
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <climits>
#include <functional>
using namespace std;
//C Headers
#include <cstdio>
#include <cstring>
#include <cstdlib>
//For Logging
//#define DISABLE_LOG
//#include "klog.h"

//Special Macros
#define SAFE_RELEASE(_X_) if(nullptr != _X_) {delete _X_; _X_ = nullptr;}

//Basic things Defines Specific to Algos only

#define ENABLE_LOG 0
#if ENABLE_LOG
static const char* spaces = "|---";
static const char* PLAYER_NAMES[] = {"MAX","MIN"}; //Let 0 is Max, 1 is Min
#define OPPOSITE_PLAYER(_p_) !(_p_)
	#define COUT_SPACES(_D_) for(int i = -1; i < _D_-1; i++) {cout<<spaces;}
	#define LOG(_X_) cout << _X_ << endl;
	#define LOG_D(_D_, _X_) COUT_SPACES(_D_) LOG(_X_);
#else
	#define COUT_SPACES(...) 
	#define LOG(...)
	#define LOG_D(...)
#endif
//enum class Player
//{
//	P_MAX,
//	P_MIN
//};
