/**********************************************
  > File Name		: debug.h
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 02 Oct 2020 09:18:17 PM CST
 **********************************************/

#ifndef _DEBUG_H
#define _DEBUG_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#ifdef NO_DEBUG
#define DEBUG(info, ...)
#else
#define DEBUG(info, ...) printf("[DEBUG] %s: %d: " info "\n", __FILE__, __LINE__, ##__VA_ARGS__) 
#endif

#define LOG_ERROR(info, ...) printf("[ERROR] %s: %d: " info "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
