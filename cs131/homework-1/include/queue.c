#include "queue.h"
#include <stdio.h>

//Implementation in C of simple Queue (FIFO data structure)

//Create and allocate a new node for process k that is waiting to get access to a certain memory module
node* createNode(int data){
	node* newNode = (node*) malloc(sizeof(node));
	newNode->process = data;
	newNode->next = NULL;
	
	return newNode;
}

//Peek and see what the front of a memory queue is.
//This peek() method will return the next process a memory module
//will give access to upoon the next memory cycle.
int peek(node** front){
	return (*front)->process;
}

//Similar to peek() but this method will remove the front of the queue
//Moving the second front most element to the front.
//This method removes the frontmost item.
int pop(node** front){
	unsigned int process = (*front)->process;
	
	node* temp = *front;
	(*front) = (*front)->next;
	free(temp);

	return process;
}

//Enqueue a new process to the back of a queue.
void push(node** front,int data){
	node* newNode = createNode(data);
	node* temp = (*front);
	
	if(temp == NULL){
		temp = newNode;
	} else {
		while(temp->next != NULL){
			temp = temp->next;
		}

		temp->next = newNode;
	}
}

//Check if a queue is empty
bool empty(node** front){
	if((*front) == NULL){
		return true;
	} 
	return false;
}

//Check if a queue has a certain process (node) already in it.
//This method is used to prevent adding a node that is already in
//the queue a second time.
bool contains(node** front,int data){
	node* cursor = (*front);

	while(cursor != NULL){
		if(cursor->process == data){
			return true;
		}

		cursor = cursor->next;
	}

	return false;
}

//Display all nodes of a queue.
void outputQueue(node** front){
	node* cursor = (*front);
	int i = 0;

	while(cursor != NULL){
		printf("Node %d, Attached Process: %d\n",++i,cursor->process);
		cursor = cursor->next;
	}
}

//Free and deallocate all nodes of a queue when the queue is no longer needed.
void destroyQueue(node** front){
	while(!(empty(front))){
		pop(front);
	}
}
