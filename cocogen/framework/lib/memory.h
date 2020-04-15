#ifndef _CCN_MEMORY_H_
#define _CCN_MEMORY_H_

extern void *mem_alloc(int size);
extern void *mem_free(void *address);
extern void *mem_copy(int size, void *mem);

#endif /* _CCN_MEMORY_H_ */
