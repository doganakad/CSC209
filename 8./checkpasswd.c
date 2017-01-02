#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Read a user id and password from standard input, 
   - create a new process to run the validate program
   - send validate the user id and password on a pipe, 
   - print a message 
        "Password verified" if the user id and password matched, 
        "Invalid password or no such user"
     depending on the return value of the validate program.
*/


int main() {
    char userid[10];
    char password[10];

    /* Read a user id and password from stdin */
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);
    int fdup[2];
    int pid;
    
    if(pipe(fdup) == -1) {
        perror("pipe");
        exit(1);
    }
    if((pid = fork()) == -1) {
        perror("fork");
        exit(1);
    } else if(pid == 0) {
        close(fdup[1]);
        dup2(fdup[0], fileno(stdin));
        execlp("validate", "validate", NULL);
        perror("exec");
        exit(1);
    } else {
        
        close(fdup[0]);
        write(fdup[1], userid, 10);
        write(fdup[1], password, 10);
        close(fdup[1]);
        int status;
        
        if((wait(&status)) == -1) {
            perror("wait");
            exit(1);
        } else {
            if(WIFEXITED(status)) {
                if(WEXITSTATUS(status) == 0){
                    printf("Password verified\n");
                } else {
                    printf("Either password or user id is incorrect\n");
                }
            }
        }
    }

    return 0;
}
