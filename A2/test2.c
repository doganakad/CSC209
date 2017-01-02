#include "qtree.h"

int main (int argc, char ** argv) {
   QNode *root = NULL, *curr;
	Node *q_list;
	int i;
    
    if (argc < 4) {
        printf ("To run the program ./test2 <name of input file> <user name> <answers>\n");
        return 1;
    }
    
    q_list = get_list_from_file (argv[1]);
    
    if (q_list == NULL) {
        printf ("The list is empty\n");
        return 1;
    }
    
    if(argc - 3 != get_list_length(q_list)) {
    	printf("The number of questions and answers do not match (must be %d)\n",get_list_length(q_list));
    	return 1;
    }
    //that is only the first test to test creation of the question tree
   root = add_next_level (root, q_list);
		
	curr = root;
	for(i = 3; i < argc; i++) {
		curr = add_user(curr,argv[2], atoi(argv[i]));	
	}
	print_qtree (root, 0);
	free_qtree(root);
    
    //your tests here
    
    return 0;
}