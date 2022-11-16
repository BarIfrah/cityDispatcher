#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

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
    char                outMsg[100] = {"WABBA LUBBA DUB DUB\n"};
    char                answer[100];
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
        printf("probably connected\n");
        //// do son op here
        rc = write(my_socket,"hello from son",100 * sizeof(char));
        printf("write?\n");
        if(rc<0)
            terminate("cannot write to server\n");
        printf("write!read?\n");
        rc = read(my_socket, &answer, 100 * sizeof(char));
        printf("11%s\n", answer);
        if(rc<0)
            terminate("cannot read from server\n");
        printf("got this from dispatch:\n%s", answer);
        rc = write(my_socket,outMsg,100 * sizeof(char));
        if(rc<0)
            terminate("cannot write to server\n");

        handleCityEvent(answer);

//        exit(EXIT_SUCCESS);
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
    printf("%d - %s about to handle an event", getpid(), title);
    sleep(5);
    printf("%d DONE!\n", getpid());
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



//int main(int argc, char *argv[])
//{
////    srand               (atoi(argv[1]));
//    int                 rc;            /* system calls return value storage */
//    int                 my_socket;
//    int                 rand_num;
//    int                 answer=1;
//    struct addrinfo     con_kind, *addr_info_res ;
//    int                 nums_sent=0,nums_deleted=0;
//
//    memset(&con_kind, 0, sizeof(con_kind)) ;
//    con_kind.ai_family = AF_UNSPEC ;        // AF_INET, AF_INET6
//    con_kind.ai_socktype = SOCK_STREAM ;
//
//    if ((rc = getaddrinfo(argv[2], argv[3],//<-server port number -argv[3]
//                          &con_kind,
//                          &addr_info_res) != 0)) {
//        fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
//        exit(EXIT_FAILURE) ;
//    }
//    my_socket = socket(addr_info_res->ai_family,        // PF_INET
//                       addr_info_res->ai_socktype,      // SOCK_STREAM
//                       addr_info_res->ai_protocol);
//    // 0 = u decide which protocal to use
//    if (my_socket < 0) {
//        perror("socket: allocation failed");
//        exit(EXIT_FAILURE) ;
//    }
//
//    rc = connect(my_socket,
//                 addr_info_res ->ai_addr,    // addr of server: IP+PORT
//                 addr_info_res ->ai_addrlen) ;
//    if (rc) {
//        perror("connect failed:");
//        exit(EXIT_FAILURE) ;
//    }
//    //loop of send random numbers to server:
//    while(answer!=FINISH){
//        rand_num=rand()%10;
//        rc = write(my_socket,&rand_num,sizeof(int));
//        nums_sent++;
//        if(rc<0)
//            terminate("cannot write to server\n");
//        rc=read(my_socket,&answer,sizeof(int));
//        if(rc<0){
//            break;
//            terminate("cannot read from server\n");
//        }
//        if(answer==YES){
//            nums_deleted++;
//        }
//    }
//    close(my_socket);
//    freeaddrinfo(addr_info_res) ;
//    printf ("%d %d\n",nums_sent,nums_deleted);
//    return EXIT_SUCCESS;
//}
