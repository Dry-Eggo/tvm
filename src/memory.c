#include <memory.h>
#include <stdio.h>

void nova_mem_zero(Heap* heap) {
    for (int i = 0; i < HEAP_SIZE; ++i) {
	heap->memory[i] = nova_mem_create_chunk_objectz(CHUNK_UNSET);
    }
}

Chunk* nova_mem_get_chunk(Heap* heap, Qword ptr) {
    return &heap->memory[ptr];
}

Qword nova_mem_alloc(Heap* heap, Qword size) {
    for (int i = 0; i <= HEAP_SIZE; ++i) {
	if (heap->memory[i].tag == CHUNK_UNSET) {
	    Chunk* chunk = &heap->memory[i];
	    chunk->tag = CHUNK_SET;
	    chunk->size = size;
	    for (int j = 0; j <= size; ++j) {
		heap->memory[i + j].tag = CHUNK_SET;
	    }
	    return i;
	}
    }
}

void nova_mem_free(Heap* heap, Qword ptr) {
    printf("Nova: Freeing: 0x%lld\n", ptr);
    Chunk* chunk = &heap->memory[ptr];
    for (int i = 0; i <= chunk->size; ++i) {
	heap->memory[ptr + i].tag = CHUNK_UNSET;
    }
    chunk->size = 0;
    chunk->tag  = CHUNK_UNSET;
}

void nova_mem_dump(Heap* heap) {
    for (int i = 0; i < HEAP_SIZE; ++i) {
	Chunk* chunk = &heap->memory[i];
	printf("Chunk %d: { size: %lld, set: ", i, chunk->size);
	if (chunk->tag == CHUNK_SET) printf("true");
	else printf("false");
	if (chunk->size > 0) {
	    for (int j = 0; j <= chunk->size; ++j, ++i) {
		Chunk* child = &heap->memory[i + j];
		printf("\tChunk-Child %d: { size: %lld, set: ", i, child->size);
		if (child->tag == CHUNK_SET) printf("true");
		else printf("false");
		printf("}\n");
	    }
	    --i;
	}
	printf("}\n");
    }
}

Chunk nova_mem_create_chunk_object(ChunkTag tag, Qword size) {
    return (Chunk) {
	.tag = tag,
	.size = size,
	.is_pointer = 0,
    };
}

Chunk nova_mem_create_chunk_objectz(ChunkTag tag) {
    return (Chunk) {
	.tag = tag,
	.size = 0,
	.is_pointer = 0,
	.value = 0,
    };
}
