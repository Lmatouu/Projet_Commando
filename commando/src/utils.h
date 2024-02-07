#include <stdio.h>
#ifndef UTILS_H
#define UTILS_H

#ifndef NDEBUG
#define TRACE(fmt, ...)                                                                   \
    do                                                                                    \
    {                                                                                     \
        fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(stderr);                                                                   \
    } while (0);

#define assert(invariant)                  \
    if (!(invariant))                      \
    {                                      \
printf ("assert failed at %d\n",__LINE__); \
abort (1);                                 \
    }
    
#else
#define TRACE(fmt, ...)

#define assert(invariant)

#endif // NDEBUG

#endif // UTILS_H
