//
//  mem_alloc.h
//
//  Created by Jinglei Ren on 12/23/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef VM_PERSISTENCE_MEM_ALLOC_H_
#define VM_PERSISTENCE_MEM_ALLOC_H_

#include <cstring>

struct MemAlloc {
  static void *Malloc(std::size_t size) { return malloc(size); }

  template <typename T>
  static void Free(T *p, std::size_t size) { free((void *)p); }

  template <typename T, typename... Arguments>
  static T *New(Arguments... args) { return new T(args...); }

  template <typename T>
  static void Delete(T *p) { return delete p; }
};

#endif // VM_PERSISTENCE_MEM_ALLOC_H_

