#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
	list_t newList = malloc(sizeof(struct list));
	newList->head = NULL;
	newList->tail = NULL;

	newList->length = 0;

	return newList;
}

void list_delete(list_t list)
{	
	struct node *temp = malloc(sizeof(struct node));
	temp = list->head;
	int i;
	for (i = 0; i < list->length; i++){
		temp = temp->next;
		free(temp->prev);
	}
	free(temp);
}

void list_insert(list_t list, int index, int data)
{
	struct node *temp;
	struct node *
newNode = malloc(sizeof(struct node));
	newNode->data = data;
	temp = list->head;
	int i;
	for (i = 0; i < index; i++)
	{
		temp = temp->next;
	}
	
	newNode->next = temp;
	newNode->prev = temp->prev;

	temp->prev = newNode;
	newNode->prev->next = newNode;

	list->length++;
}

void list_append(list_t list, int data)
{
	struct node *newNode = malloc(sizeof(struct node));
	newNode->data = data;

	if(list->length == 0){
		list->head = newNode;
		list->tail = newNode;
		list->length++;
	}
	else{
		newNode->prev = list->tail;
		list->tail = newNode;
		newNode->prev->next = newNode;
		list->length++;
	}
}

void list_print(list_t list)
{
	printf("ENTERING\n");
	if(list->length == 0){
		printf("List is empty\n");
	}
	else{
		struct node *temp;
		temp = list->head;
		int i;
		for(i = 0; i < list->length; i++){
		printf("%i\n", temp->data);
		temp = temp->next;
		}
	}
}

long list_sum(list_t list)
{
	struct node *temp;
	temp = list->head;
	int i;
	long sum = 0;
	for(i = 0; i < list->length; i++){
	sum += temp->data;
	temp = temp->next;
	}

	return sum;
}

int list_get(list_t list, int index)
{
	struct node *temp;
	temp = list->head;
	int i;
	for(i = 0; i < index; i++){
		temp = temp->next;
	}
	int wanted = temp->data;

	return wanted;
}

int list_extract(list_t list, int index)
{
	struct node *temp;
	temp = list->head;

	if(list->length == 1){
		int wanted = temp->data;
		list->head == NULL;
		list->tail == NULL;
		list->length--;
		return wanted;
	}

	int i;
	for(i = 0; i < index; i++){
		temp = temp->next;
	}
	int wanted = temp->data;

	if(index == 0){
		list->head = temp->next;
		temp->next->prev = NULL;
	}
	else if(index == list->length-1){
		list->tail = temp->prev;
		temp->prev->next = NULL;	
	}
	else{
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
	}
	list->length--;
	free(temp);
	return wanted;
}
