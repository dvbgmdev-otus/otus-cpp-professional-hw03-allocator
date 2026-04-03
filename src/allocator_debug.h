#ifndef ALLOCATOR_DEBUG_H
#define ALLOCATOR_DEBUG_H

#ifdef ALLOCATOR_DEBUG
#include <iostream>
#include <typeinfo>
#define ALLOCATOR_LOG(expr) do { expr; } while (false)
#else
#define ALLOCATOR_LOG(expr) do { } while (false)
#endif

#endif  // ALLOCATOR_DEBUG_H