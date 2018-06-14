#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct node {
	int process;
	struct node* next;
} node;

node* createNode(int data);
int peek(node** front);
int pop(node** front);
void push(node** front,int data);
bool empty(node** front);
bool contains(node** front,int data);
void outputQueue(node** front);
void destroyQueue(node** front);

#endif
