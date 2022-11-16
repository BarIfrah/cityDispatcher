#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>        // for read/write/close
#include <sys/types.h>     /* standard system types       */
#include <netinet/in.h>    /* Internet address structures */
#include <sys/socket.h>    /* socket interface functions  */
#include <netdb.h>         /* host to IP resolution       */


//========Prototypes====================================================================================================
FILE *openFile ();

void handleCityEvent (const char *);
void terminate(char *);
//======================================================================================================================
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Missing server name \n");
        exit(EXIT_FAILURE) ;
    }
    ////---socket init start
    int             rc;            /* system calls return value storage */
    int                 my_socket;
    char                outMsg[100] = {"Wait, taking care of emergency.\n"};
    char                answer1[100];
    char                answer2[100];
    struct addrinfo     con_kind, *addr_info_res ;

    memset(&con_kind, 0, sizeof(con_kind)) ;
    con_kind.ai_family = AF_UNSPEC ;        // AF_INET, AF_INET6
    con_kind.ai_socktype = SOCK_STREAM ;

    if ((rc = getaddrinfo(argv[1], argv[2], //<-server port number -argv[2]
                          &con_kind,
                          &addr_info_res) != 0)) {
        fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
        exit(EXIT_FAILURE) ;
    }
    my_socket = socket(addr_info_res->ai_family,        // PF_INET
                       addr_info_res->ai_socktype,      // SOCK_STREAM
                       addr_info_res->ai_protocol);

    if (my_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE) ;
    }

    rc = connect(my_socket,
                 addr_info_res ->ai_addr,    // addr of server: IP+PORT
                 addr_info_res ->ai_addrlen) ;
    if (rc) {
        perror("connect failed:");
        exit(EXIT_FAILURE) ;
    }
    ////---socket init end

    pid_t status;
    FILE* devNullFile = openFile();
    status = fork();
    if(status < 0) {
        perror("cannot fork()");
        exit (EXIT_FAILURE);
    }
    if(status == 0) {
        while (1) {
            //// do son op here
            rc = write(my_socket, "hello from emergency handler", 100 * sizeof(char));
            if (rc < 0)
                terminate("cannot write to server\n");

            rc = read(my_socket, &answer1, 100 * sizeof(char));
            if (rc < 0)
                terminate("cannot read from server\n");
            printf("This is an %s instance\n", answer1);

            rc = read(my_socket, &answer2, 100 * sizeof(char));
            if (rc < 0)
                terminate("cannot read from server\n");
            printf("got this from dispatch:\n%s", answer2);

            rc = write(my_socket, outMsg, 100 * sizeof(char));
            if (rc < 0)
                terminate("cannot write to server\n");

            handleCityEvent(answer1);

            printf("continue [Y/n]?\n");
            char user_choice;
            scanf(" %c", &user_choice);
            if (tolower(user_choice) == 'n')
                break;
        }
    }
    printf("DAD WAIT 1\n");
    wait(&status);
    printf("DAD WAIT 2\n");
    fclose(devNullFile);
    close(my_socket);
    freeaddrinfo(addr_info_res) ;

    return EXIT_SUCCESS;
}
//======================================================================================================================
void handleCityEvent (const char *title) {
    printf("pid: %d - about to handle an event - %s\n", getpid(), title);
    sleep(5);
    printf("pid: %d DONE!\n", getpid());
}
//======================================================================================================================
FILE *openFile ()
{
FILE* output = fopen("/dev/null", "w"); // to print nothing
if (output == NULL)
{
printf("Can't open file\n");
exit (EXIT_FAILURE);
}
return output;
}
//======================================================================================================================
//this function terminate the program if there is an error in fork
void terminate(char *error_message){
    perror (error_message);
    exit(EXIT_FAILURE);
}
//======================================================================================================================

