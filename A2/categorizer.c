#include "qtree.h"

void print_greetings();

int valid_answer(char *answer);
void print_friend_list(Node *friends, char *name);

int main (int argc, char **argv) {
	int stop;
	QNode *root = NULL;	
	char answer[MAX_LINE];	
	char name[MAX_LINE];
	
	Node * friends = NULL;
	Node * interests = NULL;

	if (argc < 2) {
        printf ("To run the program ./categorizer <name of input file>\n");
        return 1;
    }
	
    interests = get_list_from_file ( argv[1]);
	
    if (interests == NULL)
        return 1;

	root = add_next_level (root,  interests);
	free_list (interests);

	//main application loop
	stop = 0;
	while (!stop) {
		print_greetings();
		fprintf(stdout, "Please enter your name. Type 'q' to quit\n");
		fgets(name, MAX_LINE, stdin);
		name[strcspn(name, "\r\n")] = '\0';
		if(!valid_name(name)){
			if (strlen(name) > 128){
				fprintf(stderr,"Invalid name, name too long, only the first 128 characters will be used\n\n");
				name[128] = '\0';
			}
			else if(strlen(name)< 8){
				fprintf(stderr,"Invalid name, name too short\n\n");
				continue;			
			}
			else{
				fprintf(stderr,"Invalid name, name should only contain alphanumeric values\n\n");
				continue;			
			}		
		}
		friends = find_user(root, name);
		if(friends == NULL) {
			QNode *latest = root;
			while(latest !=NULL && !stop) {
				while(1) {
					fprintf(stdout,"Do you like %s? (y/n)\n", latest->question);
					fgets(answer, MAX_LINE, stdin);
					answer[strcspn(answer, "\r\n")] = '\0';
					if(valid_answer(answer))
						break;
					fprintf(stderr,"Invalid answer. The answer should be in format {yXX,nXX,qXX,YXX,NXX,QXX}\n");				
				}
				stop = (answer[0] ==	'q' || answer[0] == 'Q');
				if(!stop){
					latest = add_user(latest, name, answer[0] == 'y' || answer[0] == 'Y');				
				}		
			}
			if(!stop)
				friends = find_user(root,name);
		}
		if(!stop) {
			print_friend_list(friends, name);
			fprintf(stdout, "\n");		
		}			
	}
    print_qtree (root, 0);
    free_qtree(root);
	return 0;
}

void print_greetings () {
	printf ("----------------------------------------------\n");
	printf ("Friend recommender system. Find people who are just like you!\n");
	printf ("CSC209 fall 2016 team. All rights reserved\n");
	printf ("----------------------------------------------\n");
}


int valid_answer(char *answer) {
	if(strlen(answer) > 3)
		return 0;
	return (answer[0] == 'q' || answer[0] == 'y' || answer[0] == 'n' || answer[0] == 'Q' || answer[0] == 'Y' || answer[0] == 'N');
}

void print_friend_list(Node *friends,char *name){
	int len = get_list_length(friends) - 1;
	if(len == 0) {
		fprintf(stdout, "Sorry,no users with similar interests joined yet\n");	
	}
	else {
		printf("friend recommendations for user %s:\n", name);
		while (friends != NULL){
			if(strcmp(friends->str, name) != 0) {
				fprintf(stdout,"%s\n", friends->str);			
			}
			friends = friends->next;		
		}
		fprintf(stdout,"You have total %d potential friend(s) !!!\n", len);	
	}
}