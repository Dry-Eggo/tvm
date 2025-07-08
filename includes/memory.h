#pragma once

#include <types.h>
#define HEAP_SIZE 100

typedef enum: Byte {
    CHUNK_SET,
    CHUNK_UNSET,
} ChunkTag;

typedef struct {
    ChunkTag tag;
    Qword size;
    Byte  is_pointer;
    Qword value;
} Chunk;

typedef struct {
    Chunk memory[HEAP_SIZE];
} Heap;

void nova_mem_zero(Heap* heap);
Qword nova_mem_alloc(Heap* heap, Qword size);
void nova_mem_dump(Heap* heap);
void nova_mem_free(Heap* heap, Qword size);
Chunk nova_mem_create_chunk_object(ChunkTag tag, Qword size);
Chunk nova_mem_create_chunk_objectz(ChunkTag tag);
