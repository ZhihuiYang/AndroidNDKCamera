#pragma once

#include <android/log.h>

#define MY_LOG_LEVEL_VERBOSE 	1
#define MY_LOG_LEVEL_DEBUG 		2
#define MY_LOG_LEVEL_INFO		3
#define MY_LOG_LEVEL_WARING		4
#define MY_LOG_LEVEL_ERROR		5
#define MY_LOG_LEVEL_FATAL		6
#define MY_LOG_LEVEL_SILENT		7

#ifndef MY_LOG_TAG
#define MY_LOG_TAG __FILE__
#endif

#if defined(LOG_LEVEL_DEBUG)
#define MY_LOG_LEVEL MY_LOG_LEVEL_DEBUG
#elif defined(LOG_LEVEL_INFO)
#define MY_LOG_LEVEL MY_LOG_LEVEL_INFO
#elif defined(LOG_LEVEL_WARING)
#define MY_LOG_LEVEL MY_LOG_LEVEL_WARING
#else
#define MY_LOG_LEVEL MY_LOG_LEVEL_DEBUG
#endif

#define MY_LOG_NOOP (void) 0

#define MY_LOG_PRINT(level, fmt, ...) \
	__android_log_print(level, MY_LOG_TAG, "(%s:%u) %s: \n" fmt,\
			__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#if MY_LOG_LEVEL_VERBOSE >= MY_LOG_LEVEL
#define LOG_VERBOSE(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
#define LOG_VERBOSE(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_DEBUG >= MY_LOG_LEVEL
#define LOG_DEBUG(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_INFO >= MY_LOG_LEVEL
#define LOG_INFO(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_WARNING >= MY_LOG_LEVEL
#define LOG_WARNING(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_WARNING, fmt, ##__VA_ARGS__)
#else
#define LOG_WARNING(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_ERROR >= MY_LOG_LEVEL
#define LOG_ERROR(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_FATAL >= MY_LOG_LEVEL
#define LOG_FATAL(fmt, ...) \
	MY_LOG_PRINT(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
#define LOG_FATAL(...) MY_LOG_NOOP
#endif

#if MY_LOG_LEVEL_SILENT >= MY_LOG_LEVEL
#define LOG_ASSERT(expression, fmt, ...) \
	if (!(expression)) \
	{ \
		__android_log_assert(#expression, MY_LOG_TAG, \
				fmt, ##__VA_ARGS__); \
	}
#else
#define LOG_ASSERT(...) MY_LOG_NOOP
#endif

