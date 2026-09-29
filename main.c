#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/mman.h>
#define _ALIGN_VALUE(x, align) ((x) - 1) / (align) + 1
#define first() list->data
#define last() last->before->data
struct VectorItem {
	struct VectorItem* before;
	void* data;
	struct VectorItem* next;
};
typedef struct Vector {
	struct VectorItem* last;
	struct VectorItem* list;
	void(*push_back)(void*);
	void*(*at)(int);
} Vector;
void VecPushBack(void* d, Vector* x, void*(*_malloc)(size_t)) {
	x->last->data = d;
	x->last->next = _malloc(sizeof(struct VectorItem));
	x->last->next->before = x->last;
	x->last = x->last->next;
}
void VecPushBackEnd(void* d __attribute__((unused)), Vector* x __attribute__((unused)),
	void*(*_malloc)(size_t) __attribute__((unused))) {}
void* VecAt(int val, Vector* x) {
	struct VectorItem* n = x->list;
	for(int a = 0; a < val; a++)
		n = n->next;
	return n->data;
}
void* VecAtEnd(int val __attribute__((unused)), Vector* x __attribute__((unused))) { return NULL; }
void VecInit(Vector* x) {
	int p_size = getpagesize();
	x->list = malloc(sizeof(struct VectorItem));
	x->last = x->list;
	int padding1 = sizeof(void*) == 4 ? 10 : 20;
	int padding2 = sizeof(void*) == 4 ? 5 : 10;
	// push_back
	char* pb = aligned_alloc(
		_ALIGN_VALUE(VecPushBackEnd-VecPushBack+padding1, p_size) * p_size, p_size);
	if(!pb) perror("Couldn't allocate push_back function\n");
	#if UINTPTR_MAX == 0xffffffff
		*(uint32_t*)(pb+1) = (uint32_t)x;
		pb[0] = 0xbe;
		*(uint32_t*)(pb+6) = (uint32_t)malloc;
		pb[5] = 0xba;
	#else
		*(uint64_t*)(pb+2) = (uint64_t)x;
		pb[0] = 0x48; pb[1] = 0xbe;
		*(uint64_t*)(pb+12) = (uint64_t)malloc;
		pb[10] = 0x48; pb[11] = 0xba;
	#endif
	memcpy(pb+padding1, VecPushBack, VecPushBackEnd-VecPushBack);
	if(mprotect(pb, _ALIGN_VALUE(VecPushBackEnd-VecPushBack, p_size) * p_size, PROT_EXEC))
		perror("Couldn't add push_back protections\n");
	x->push_back = (void(*)(void*))pb;
	// at
	pb = aligned_alloc(
		_ALIGN_VALUE(VecAtEnd-VecAt+padding2, p_size) * p_size, p_size);
	if(!pb) perror("Couldn't allocate at function\n");
	#if UINTPTR_MAX == 0xffffffff
		*(uint32_t*)(pb+1) = (uint32_t)x;
		pb[0] = 0xbe;
	#else
		*(uint64_t*)(pb+2) = (uint64_t)x;
		pb[0] = 0x48; pb[1] = 0xbe;
	#endif
	memcpy(pb+padding2, VecAt, VecAtEnd-VecAt);
	if(mprotect(pb, _ALIGN_VALUE(VecAtEnd-VecAt, p_size) * p_size, PROT_EXEC))
		perror("Couldn't add at protections\n");
	x->at = (void*(*)(int))pb;
}
int main() {
	Vector x;
	VecInit(&x);
	x.push_back((void*)12);
	x.push_back((void*)15);
	x.push_back((void*)17);
	printf("%ld\n", (intptr_t)x.first());
	printf("%ld\n", (intptr_t)x.last());
	printf("%ld\n", (intptr_t)x.at(1));
}
