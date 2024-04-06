/*
Student No.: 109550060
Student Name: 陳星宇
Email: borachen0621@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define init_size 20000
int called = 0;

typedef struct __attribute__((packed)) B{
    size_t size;
    int used;
    struct B *prev;
    struct B *next;
} Block;

void* mem;
Block *head;

void *malloc(int size){
    if(size>init_size) return NULL;
    if(size==0){
        Block* cur = head;
        int largest = 0;
        while(cur!=NULL){
            if(cur->size>largest && !cur->used) largest = cur->size;
            cur = cur->next;
        }
        char buffer[27];
        snprintf(buffer, sizeof(buffer), "Max Free Chunk Size = %d\n", largest);
        write(1, buffer, strlen(buffer));
        munmap(mem, init_size);
        return NULL;
    }
    size = (size/32+(size%32==0?0:1))*32;
    if(called==0){
        called = 1;
        mem = mmap(NULL, init_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        head = (Block*) mem;
        head->size = init_size-32;
        head->used = 0;
        head->prev = NULL;
        head->next = NULL;
    }

    Block *cur = head;
    while(cur!=NULL){
        if(!cur->used && cur->size>=size){
            break;
        }
        cur = cur->next;
    }

    if(cur->size-size>32){ // able to be the next new chunk
        Block* newBlock = (Block*)((void*)cur + 32 + size);
        newBlock->size = cur->size - size - 32;
        newBlock->used = 0;
        
        newBlock->next = cur->next;
        if(cur->next!=NULL){
            newBlock->next->prev = newBlock;
        }
        cur->next = newBlock;
        newBlock->prev = cur;
        cur->size = size;
    }
    cur->used = 1;
    return (void*)cur+32;
}

void free(void *ptr){
    Block* cur = (Block*) (ptr - 32);

    if(cur->prev!=NULL && !cur->prev->used){ // merge with left
        if(cur->next!=NULL && !cur->next->used){ // merge with right
            Block* next = cur->next;
            cur->prev->next = next->next;
            cur->prev->size += cur->size+next->size+64;
            if(next->next!=NULL){
                next->next->prev = cur->prev;
            }
        }
        else { // no merge with right
            cur->prev->next = cur->next;
            cur->prev->size += 32+cur->size;
            if(cur->next!=NULL){
                cur->next->prev = cur->prev;
            }
        }
    }
    else{ // no merge with left
        cur->used = 0;
        if(cur->next!=NULL && !cur->next->used){ // merge with right
            Block* next = cur->next;
            cur->next = next->next;
            cur->size += 32+next->size;
            if(next->next!=NULL){
                next->next->prev = cur;
            }
        }
    }
}