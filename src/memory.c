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
    int chunks_needed = (size + 7) / 8;
    for (int i = 0; i <= HEAP_SIZE - chunks_needed; ++i) {
	int can_allocate = 1;
	for (int j = 0; j < chunks_needed; ++j) {
	    if (heap->memory[i + j].tag != CHUNK_UNSET) {
		can_allocate = 0;
		break;
	    }
	}
	if (can_allocate) {
	    Chunk* head = &heap->memory[i];
	    head->tag = CHUNK_SET;
	    head->size = size;
	    head->is_pointer = 0;
	    head->value = 0;
	

	    for (int j = 0; j < chunks_needed; ++j) {
		heap->memory[i + j].tag = CHUNK_SET;
	    }
	    printf("Nova: Allocated %lld bytes... Span %d chunks at 0x%d\n", size, chunks_needed, i);
	    return i;
	}
    }
}

void nova_mem_free(Heap* heap, Qword ptr) {
    printf("Nova: Freeing: 0x%lld\n", ptr);
    Chunk* chunk = &heap->memory[ptr];
    int chunks_to_free = (chunk->size + 7) / 8;
    for (int i = 0; i < chunks_to_free; ++i) {
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
	    int child_chunks = (chunk->size + 7)/8;
	    for (int j = 0; j < child_chunks; ++j, ++i) {
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
