/*
 * mymalloc.h
 *
 *  Created on: Jul 11, 2016
 *      Author: eyngzui
 */

#ifndef INCLUDE_MYMALLOC_H_
#define INCLUDE_MYMALLOC_H_

//#ifdef _DEBUG
#include <malloc.h>
#include <stddef.h>
#include "logger.h"

#define PRINT LOG_DEBUG

static const char *fname_delete;
static const char *funcname_delete;
static int lineno_delete;

//for C++
inline void *operator new(unsigned int size, const char* file,
        const char* funcName,
        int line)
{
    PRINT("operator new : %d Bytes in %s, %s, %d\n", size, file, funcName,
            line);
    void *p = operator new(size);
    if (!p)
    {
        PRINT("out of memory in %s, %s, %d\n", file, funcName, line);
    }
    PRINT("operator new : %p\n", p);
    return p;
}

inline void *operator new[](unsigned int size, const char* file,
        const char* funcName,
        int line)
{
    PRINT("operator new[] : %d Bytes in %s, %s, %d\n", size, file, funcName,
            line);
    void * p = operator new[](size);
    if (!p)
    {
        PRINT("out of memory in %s, %s, %d\n", file, funcName, line);
    }
    PRINT("operator new[] : %p\n", p);
    return p;
}

inline void deletep(const char* file, const char* funcName, int line)
{
    fname_delete = file;
    funcname_delete = funcName;
    lineno_delete = line;
}

inline void operator delete(void *p)
{
    PRINT("operator delete : %p\n in %s, %s, %d", p, fname_delete,
            funcname_delete, lineno_delete);
    free(p);
}

inline void operator delete[](void *p)
{
    PRINT("operator delete[] : %p\n in %s, %s, %d", p, fname_delete,
            funcname_delete, lineno_delete);
    free(p);
}

#define new  new(__FILE__, __FUNCTION__, __LINE__)
#define delete deletep(__FILE__, __FUNCTION__, __LINE__),delete
//end for C++

//for C
inline void * myMalloc(size_t size, const char* file, const char* funcName,
        int line)
{
    PRINT("malloc %d Bytes in %s, %s, %d\n", size, file, funcName, line);
    void *p = malloc(size);
    if (p == NULL)
    {
        PRINT("out of memory in %s, %s, %d\n", file, funcName, line);
    }
    PRINT("malloc : %p\n", p);
    return p;
}

inline void myFree(void *p, const char* file, const char* funcName, int line)
{
    PRINT("free : %p , in %s, %s, %d\n", p, file, funcName, line);
    free(p);
}

#define malloc(n) myMalloc(n, __FILE__, __FUNCTION__, __LINE__)
#define free(p) myFree(p, __FILE__, __FUNCTION__, __LINE__)

//#endif /*_DEBUG*/
#endif /* INCLUDE_MYMALLOC_H_ */
