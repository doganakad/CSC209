#include "qtree.h"

QNode *add_next_level (QNode *current, Node *list_node) {
	int str_len;
	
	str_len = strlen (list_node->str);
	current = (QNode *) calloc (1, sizeof(QNode));

	current->question =  (char *) calloc (str_len +1, sizeof(char ));
	strncpy ( current->question, list_node->str, str_len );
	current->question [str_len] = '\0';  
	current->node_type = REGULAR;
	
	if (list_node->next == NULL) {
		current->node_type = LEAF;
		return current;
	}
	
	current->children[0].qchild = add_next_level ( current->children[0].qchild, list_node->next);
	current->children[1].qchild = add_next_level ( current->children[1].qchild, list_node->next);

	return current;
}

void print_qtree (QNode *parent, int level) {
	int i;
	for (i=0; i<level; i++)
		printf("\t");
	
	printf ("%s type:%d\n", parent->question, parent->node_type);
	if(parent->node_type == REGULAR) {
		print_qtree (parent->children[0].qchild, level+1);
		print_qtree (parent->children[1].qchild, level+1);
	}
	else { //leaf node
		for (i=0; i<(level+1); i++)
			printf("\t");
		print_users (parent->children[0].fchild);
		for (i=0; i<(level+1); i++)
			printf("\t");
		print_users (parent->children[1].fchild);
	}
}

void print_users (Node *parent) {
	if (parent == NULL)
		printf("NULL\n");
	else {
		printf("%s, ", parent->str);
		while (parent->next != NULL) {
			parent = parent->next;
			printf("%s, ", parent->str);
		}
		printf ("\n");
	}
}

void free_qtree(QNode * root){
	if(root->node_type == LEAF){
		free_list(root->children[0].fchild);
		free_list(root->children[1].fchild);
	}
	else {
		free_qtree(root->children[0].qchild);
		free_qtree(root->children[1].qchild);	
	}
	free(root->question);
	free(root);
	}

QNode *add_user(QNode *current, char *name, int answer) {
	while(1) {
		if(!valid_name(name)){
			if(strlen(name) > 128) {
				fprintf(stderr, "Name too long, only the first 128 characters will be used\n");
				name[128] = '\0';
				break;
			}
			else if(strlen(name) < 8) {
				fprintf(stderr, "Name too short, the length should at least be 8\n");
				exit(0);
			}	
			else{
				fprintf(stderr,"Name should only contain alphanumeric values\n");
				exit(0);
			}	
		}	
		break;
	}
	union Child *child = &current->children[answer];
	if (current->node_type == LEAF){
		Node *node = (Node *)malloc(sizeof(Node));
		node->str = (char *)malloc(strlen(name) +1);
		strcpy(node->str, name);
		node->next = child->fchild;
		child->fchild = node;
		return NULL;	
	}
	return child->qchild;
}

Node *find_user(QNode *current, char *name){
	int i;
	if (current->node_type == LEAF){
		for(i = 0; i < 2; i++){
			Node *it = current->children[i].fchild;
			while (it !=NULL && strcmp(it->str, name) != 0){
				it = it->next;			
			}
			if (it !=NULL){
				return current->children[i].fchild;
			}		
		}	
	}
	else{
		Node *r;
		for(i = 0; i < 2; i++){
			if((r = find_user(current->children[i].qchild, name)) != NULL) {
				return r;			
			}		
		}	
	}
	return NULL;
}

int valid_name(char *name){
	int i;
	int len = strlen(name);
	if(len < 8 || len > 128)
		return 0;
	for(i = 0; i < len; i++){
		if(!(name[i] >= '0' && name[i] <= '9') && !((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z')))
			return 0;
	}
	return 1;
}
