#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "questions.h"

Node * get_list_from_file (char *input_file_name) {
	Node * head = NULL;
	char line[MAX_LINE];
	FILE *fp;
	Node * tail = NULL;
	fp = fopen(input_file_name, "r");
	if(fp == NULL) {
		fprintf(stderr, "error opening file\n");
	}
    while(fgets(line, MAX_LINE, fp)!=NULL) {
    	line[strcspn (line, "\r\n")] = '\0';
		Node *y = new_node(line);
	
		if(head == NULL) {
			head = y;
			tail = y;
		}
		tail->next = y;
		tail=y;	
   }
   fclose(fp);
	return head;
}
Node *new_node(char *str) {
	if(str == NULL) {
		return NULL;
	}
	Node *i = malloc(sizeof(Node)*1);
	size_t len = strlen(str);
	i->str = malloc(len+1);
	strcpy(i->str,str);
	return i;
}
void print_list (Node *head) {
	Node *current = head;
	while (current!= NULL) {
		printf("%s\n", current->str);
		current = current->next;
	}
	
}

void free_list (Node *head) {
	Node *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

int get_list_length(Node *head) {
	int i = 0;
	while (head!=NULL) {
		i++;
		head = head->next;
	}
	return i;
}
