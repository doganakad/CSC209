#ifndef QUESTIONS_H
#define QUESTIONS_H

#define MAX_LINE 256

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct str_node {
	char *str;
	struct str_node *next;
} Node;

Node * get_list_from_file (char *input_file_name);
Node * new_node(char *str);
void print_list (Node *current);
void free_list (Node *head);
int get_list_length(Node* head);

#endif
