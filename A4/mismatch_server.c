/*
 * demonstration server, sockets and all that.
 * Does some i/o with each client, and copies other clients on the output.
 * Detects disconnections, uses select() properly, etc.
 *
 * Alan J Rosenthal, December 2000
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include "qtree.h"
#define INPUT_ARG_MAX_NUM 3
#define DELIM " \r\n"

#ifndef PORT
	#define PORT 12345
#endif

static int listenfd;

struct client {
	int fd;
	struct in_addr ipaddr;
	struct client *next;
}*top = NULL;
int howmany = 1; /* server counts as one person who knows the muffin man */

static char greeting[] =
		"----------------------------------------------\r\n"
				"Online dating service recommender system. Find people who are your best mismatches!\r\n"
				"CSC209 fall 2016 team. All rights reserved\r\n"
				"----------------------------------------------\r\n";
static char user_input[] = "What is your username?\r\n";
static char too_long_username[] =
		"Invalid name, name too long, only the first 128 characters will be used\r\n";
static char enter_command[] = "Enter command >>\r\n";

static void addclient(int fd, struct in_addr addr);
static void removeclient(int fd);
static void broadcast(char *s, int size);
int tokenize(char * cmd, char **cmd_argv);

int valid_answer(char *answer);
void print_friend_list(Node *friends, char *name);

int tokenize(char *cmd, char **cmd_argv) {
	int cmd_argc = 0;
	char *next_token = strtok(cmd, DELIM);

	while (next_token != NULL) {
		cmd_argv[cmd_argc] = next_token;
		++cmd_argc;

		if (cmd_argc < (INPUT_ARG_MAX_NUM - 1))
			next_token = strtok(NULL, DELIM);
		else
			break;
	}

	if (cmd_argc == (INPUT_ARG_MAX_NUM - 1)) {
		cmd_argv[cmd_argc] = strtok(NULL, "");
		if (cmd_argv[cmd_argc] != NULL)
			++cmd_argc;
	}

	return cmd_argc;
}

int main(int argc, char **argv) {
	struct client *p;
	extern void bindandlisten(), whatsup(struct client *p);

	//int stop;
	QNode *root = NULL;
	char answer[MAX_LINE];
	char name[MAX_LINE];
	//char message[MAX_LINE];
	char **cmd_argv;
	Node * friends = NULL;
	Node * interests = NULL;
	cmd_argv = malloc(sizeof(char*) * 5);
	if (argc < 2) {
		printf("To run the program ./categorizer <name of input file>\n");
		return 1;
	}

	interests = get_list_from_file(argv[1]);

	int number_of_questions = get_list_length(interests);

	if (interests == NULL)
		return 1;

	root = add_next_level(root, interests);
	free_list(interests);

	bindandlisten(); /* aborts on error */

	/* the only way the server exits is by being killed */
	while (1) {
		fd_set fdlist;
		int maxfd = listenfd;
		FD_ZERO(&fdlist);
		FD_SET(listenfd, &fdlist);
		for (p = top; p; p = p->next) {
			FD_SET(p->fd, &fdlist);
			if (p->fd > maxfd)
				maxfd = p->fd;
		}
		if (select(maxfd + 1, &fdlist, NULL, NULL, NULL) < 0) {
			perror("select");
		} else {
			for (p = top; p; p = p->next)
				if (FD_ISSET(p->fd, &fdlist))
					break;
			/*
			 * it's not very likely that more than one client will drop at
			 * once, so it's not a big loss that we process only one each
			 * select(); we'll get it later...
			 */
			if (p)
				whatsup(p); /* might remove p from list, so can't be in the loop */
			if (FD_ISSET(listenfd, &fdlist)) { /* a new client is connecting*/
				int fd;
				struct sockaddr_in r;
				socklen_t socklen = sizeof r;

				if ((fd = accept(listenfd, (struct sockaddr *) &r, &socklen))
						< 0) {
					perror("accept");
				} else {
					printf("connection from %s\n", inet_ntoa(r.sin_addr));
					addclient(fd, r.sin_addr);

					/* sing */
					broadcast(greeting, sizeof greeting - 1);
					broadcast(user_input, sizeof user_input - 1);

					fd_set fdlist;
					FD_ZERO(&fdlist);
					FD_SET(fd, &fdlist);
					select(fd + 1, &fdlist, NULL, NULL, NULL);

					read(fd, name, sizeof name);
					name[strcspn(name, "\r\n")] = '\0';

					if (strlen(name) > 128) {
						broadcast(too_long_username,
								sizeof too_long_username - 1);
						name[128] = '\0';
					}

					char command[MAX_LINE];
					broadcast(enter_command, strlen(enter_command));
					while (1) {
						int len = read(fd, command, sizeof command);
						command[len] = '\0';
						int cmd_argc = tokenize(command, cmd_argv);
						// /sprintf(reply, "Command used: %s\n", command);
						// broadcast(reply, strlen(reply));
						if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
							removeclient(fd);
							close(fd);
							break;
						}
						if (strcmp(cmd_argv[0], "get_all") == 0
								&& cmd_argc == 1) {
							friends = find_user(root, name);
							if (friends == NULL)
								broadcast("You have not taken the test yet\n",
										strlen(
												"You have not taken the test yet\n"));
							else {
								friends = find_mismatch_user(root, name,
										number_of_questions);
								print_friend_list(friends, name);
							}
							continue;
						} else if (strcmp(cmd_argv[0], "do_test") == 0
								&& cmd_argc == 1) {
							friends = find_user(root, name);
							if (friends != NULL)
								broadcast("You have already taken the test\n",
										strlen(
												"You have already taken the test\n"));
							else {
								QNode *latest = root;
								while (latest != NULL) {
									// traverse through the question tree.
									char the_question[] =
											"Do you like %s (y/n)?\r\n";
									char latest_question[MAX_LINE];
									sprintf(latest_question, the_question,
											latest->question);
									broadcast(latest_question,
											strlen(latest_question));
									int len = read(fd, answer,
											sizeof answer - 1);
									answer[len] = '\0';
									printf("%s\n", answer);
									if (strcmp(answer, "quit") == 0) {
										removeclient(fd);
										close(fd);
										break;
									}
									if (valid_answer(answer))
										latest = add_user(latest, name,
												answer[0] == 'y'
														|| answer[0] == 'Y');
								}
								char complete[] = "Test complete\r\n";
								write(fd, complete, strlen(complete));
							}
							continue;
						} else if (strcmp(cmd_argv[0], "post") == 0
								&& cmd_argc == 3) {
							printf("Message from %s: %s\r\n", name,
									cmd_argv[2]);
							continue;
						} else {
							write(fd, "Incorrect synthax\r\n",
									strlen("Incorrect synthax\r\n"));
							continue;
						}
					}
				}
			}
		}
	}

	print_qtree(root, 0);
	free_qtree(root);
	return 0;
}

void bindandlisten() /* bind and listen, abort on error */
{
	struct sockaddr_in r;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	memset(&r, '\0', sizeof r);
	r.sin_family = AF_INET;
	r.sin_addr.s_addr = INADDR_ANY;
	r.sin_port = htons(PORT);
	printf("Listening on %d\n", PORT);

	int enable = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))
			< 0)
		perror("setsockopt(SO_REUSEADDR) failed");

	if (bind(listenfd, (struct sockaddr *) &r, sizeof r)) {
		perror("bind");
		exit(1);
	}

	if (listen(listenfd, 5)) {
		perror("listen");
		exit(1);
	}
}

void whatsup(struct client *p) /* select() said activity; check it out */
{
	char garbage[80];
	int len = read(p->fd, garbage, sizeof garbage);
	if (len > 0) {
	} else if (len == 0) {
		char buf[80];
		printf("Disconnect from %s\n", inet_ntoa(p->ipaddr));
		fflush(stdout);
		sprintf(buf, "Goodbye %s\r\n", inet_ntoa(p->ipaddr));
		removeclient(p->fd);
		broadcast(buf, strlen(buf));
	} else {
		/* shouldn't happen */
		perror("read");
	}
}

static void addclient(int fd, struct in_addr addr) {
	struct client *p = malloc(sizeof(struct client));
	if (!p) {
		fprintf(stderr, "out of memory!\n"); /* highly unlikely to happen */
		exit(1);
	}
	printf("Adding client %s\n", inet_ntoa(addr));
	fflush(stdout);
	p->fd = fd;
	p->ipaddr = addr;
	p->next = top;
	top = p;
	howmany++;
}

static void removeclient(int fd) {
	struct client **p;
	for (p = &top; *p && (*p)->fd != fd; p = &(*p)->next)
		;
	if (*p) {
		struct client *t = (*p)->next;
		printf("Removing client %s\n", inet_ntoa((*p)->ipaddr));
		fflush(stdout);
		free(*p);
		*p = t;
		howmany--;
	} else {
		fprintf(stderr, "Trying to remove fd %d, but I don't know about it\n",
				fd);
		fflush(stderr);
		exit(1);
	}
}

static void broadcast(char *s, int size) {
	struct client *p;
	int len;
	for (p = top; p; p = p->next)
		if ((len = write(p->fd, s, size)) != size) {
			removeclient(p->fd);
			perror("write");
		}
	/* should probably check write() return value and perhaps remove client */
}

int valid_answer(char *answer) {
	if (strlen(answer) > 3)
		return 0;
	return (answer[0] == 'q' || answer[0] == 'y' || answer[0] == 'n'
			|| answer[0] == 'Q' || answer[0] == 'Y' || answer[0] == 'N');
}

void print_friend_list(Node *friends, char *name) {
	int len = get_list_length(friends);
	if (len == 0) {
		char mismatch_question[] =
				"No completing personalities found. Please try again later.\r\n";
		broadcast(mismatch_question, strlen(mismatch_question));
	} else {
		char mismatch[] = "Here are your best mismatches %s:\n";
		char mismatch_recommendation[MAX_LINE];
		sprintf(mismatch_recommendation, mismatch, name);
		broadcast(mismatch_recommendation, strlen(mismatch_recommendation));
		while (friends != NULL) {
			if (strcmp(friends->str, name) != 0) {
				broadcast(friends->str, strlen(friends->str));
				broadcast("\n", strlen("\n"));
			}
			friends = friends->next;
		}
	}
}

