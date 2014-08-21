//
//  COMP1927 Assignment 1 - Memory Suballocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014
//  Copyright (c) 2012-2014 UNSW. All rights reserved.
//

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEADER_SIZE	 sizeof(struct free_list_header)  
#define MAGIC_FREE	  0xDEADBEEF
#define MAGIC_ALLOC	 0xBEEFDEAD

typedef unsigned char byte;
typedef u_int32_t vlink_t;
typedef u_int32_t vsize_t;
typedef u_int32_t vaddr_t;

typedef struct free_list_header
{
	u_int32_t magic;			  // ought to contain MAGIC_FREE
	vsize_t size;				  // # bytes in this block (including header)
	vlink_t next;				  // memory[] index of next free block
	vlink_t prev;				  // memory[] index of previous free block
} free_header_t;

// Global data

static byte *memory = NULL;	// pointer to start of suballocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;	// number of bytes malloc'd in memory[]


//======================================
//My Declarations
static int powerOfTwo (u_int32_t n);
static void splitChunk (free_header_t *chunk);
static void printHeader (free_header_t *p);
//=====================================

void sal_init(u_int32_t size)
{

	//fprintf(stderr, "sal_init: insufficient memory");
	//abort();

	int count = powerOfTwo(size);
	//initialise global variables
	memory_size = count;
	memory = malloc(count);
	free_list_ptr = 0;
	
	//initialise first free_list_header	
	free_header_t *first = (free_header_t *) memory;
	first->magic = MAGIC_FREE;
	first->size = memory_size;
	first->next = free_list_ptr;
	first->prev = free_list_ptr;

}

void *sal_malloc(u_int32_t n)
{
	byte *allocated;

	//iterate through to get to the first free space
	free_header_t *first = (free_header_t *) memory+free_list_ptr;
	free_header_t *curr = first;
	int circle = TRUE;
	
	printHeader(curr);
	
	while (circle)) {
		//found a chunk that is bigger than we need
		if (n <= HEADER_SIZE+curr->size) {
			//check if we can break down chunks
			if (n <= HEADER_SIZE+(curr->size)/2) {
				//break chunk, then salloc again (and hence find that chunk)
				//TODO this is a bad way i think...
				splitChunk(curr);
				allocated = sal_malloc(u_int32_t n/2)
			//if cannot splt, then five them this bit of memory
			} else {
				allocated = (byte *)curr + HEADER_SIZE;
			}
			break;
		//else if we get back around the full circle, nothing is big enough so error
		} else if (curr == first) {
			fprintf(stderr, "sal_init: insufficient memory\n");
			allocated = NULL;
			circle = FALSE;
			break;
		}
		curr += curr->next;
	}
	
	
	//create new allocated header
	/*
	free_header_t *new = (free_header_t *) memory;
	new->magic = 
	new->size = 
	new->next = 
	new->prev = 
	*/
	//clean up shit
	
	return allocated;
}

void sal_free(void *object)
{
	//delete header
	
	//merge regions
}

void sal_end(void)
{
  free(memory);
}

void sal_stats(void)
{
	// Optional, but useful
	printf("sal_stats\n");
	 // we "use" the global variables here
	 // just to keep the compiler quiet
	memory = memory;
	free_list_ptr = free_list_ptr;
	memory_size = memory_size;
}


//===================================
//		  My functions
//===================================

static void splitChunk (free_header_t *chunk)
{
	//printf("split at %d\n",((byte *)curr + curr->size - (byte *)memory));
	printf("splitting chunk %d at %d\n",((int)chunk - (int)memory), ((int)chunk - (int)memory + chunk->size/2));

	//create new chunk header
	free_header_t *new = (byte *)chunk + chunk->size/2; 
	new->magic = MAGIC_FREE;
	new->size = chunk->size/2;
	new->next = chunk->next;
	new->prev = (byte *)chunk-memory;

	//modufy the old R side
	free_header_t *chunkNext = //TODO;
	chunkNext->prev = (byte *)new-memory;

	//modify the old L side
	chunk->size = chunk->size/2;
	chunk->next = (byte *)chunk+size/2;
	
	printf("new\n");
	printHeader(new);
	printf("chunk\n");
	printHeader(chunk);
	printf("chunkNext\n");
	printHeader(chunkNext);
}

static void printHeader (free_header_t *p)
{	
	printf("printing header information at index memory[%d]\n",(int)p-(int)memory);
	printf("p is %p\n",p);	
	printf("magic number is %d\n", p->magic);
	printf("size is %d\n", p->size);
	printf("next is %d\n", p->next);
	printf("prev is %d\n", p->prev);
}

static int powerOfTwo (u_int32_t n)
{
	int count = 1;
	while (count < n) {
		 count *= 2;
	}
	return count;
}

void main (int argc, char *argv[])
{
	sal_init(128);
	printf("MAGIC_FREE IS %d\n", MAGIC_FREE);
	printf("MAGIC_USED IS %d\n", MAGIC_ALLOC);
	printf("MEMORY IS %p\n",memory);
	//free_list_ptr = 10;
	byte *john = memory+free_list_ptr;
	
	sal_malloc(60);
	printf("salloc2===============\n");
	sal_malloc(60);
}
