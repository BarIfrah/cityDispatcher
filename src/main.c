#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // for memset
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <syslog.h>
#include <ctype.h>

const char *department[] = {"Police", "Ambulance", "Fire Dept", "Maintenance"};
const char *emergency[] = {"Murder", "CPR", "Child birth", "Struck train", "Flood",
                           "Stack overflow"};

//======================================================================================================================

#define NUM_OF_CLIENTS 4

//======================================================================================================================
void terminate(char *);

//======================================================================================================================
int main(int argc, char *argv[]) {
    srand(time(NULL));
    int return_code;                // return code
    int main_socket;
    int serving_socket;
    int fd;
    char outmsg[100] = "Dispatching message for you.\n";
    char outmsg2[100];
    char inmsg[100];
    fd_set rfd, c_rfd;

    struct sockaddr_storage her_addr;
    socklen_t her_addr_size;
    struct addrinfo con_kind, *addr_info_res;
    time_t time_before, time_after, total_time;
    int i = 0;


    memset(&con_kind, 0, sizeof(con_kind));
    con_kind.ai_family = AF_UNSPEC;
    con_kind.ai_socktype = SOCK_STREAM;
    con_kind.ai_flags = AI_PASSIVE;        // system will fill my IP

    if ((return_code = getaddrinfo(NULL,            // NULL = you set IP address
                                   argv[1],
                                   &con_kind,
                                   &addr_info_res)) != 0) {
        fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(return_code));
        exit(EXIT_FAILURE);
    }

    main_socket = socket(addr_info_res->ai_family,
                         addr_info_res->ai_socktype,
                         addr_info_res->ai_protocol);

    if (main_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE);
    }

    return_code = bind(main_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);

    if (return_code) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    return_code = listen(main_socket, NUM_OF_CLIENTS); //listen to clients
    if (return_code) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }


    her_addr_size = sizeof(her_addr);

    FD_ZERO(&rfd);
    FD_SET(main_socket, &rfd);
//    time_before = time(NULL);

    c_rfd = rfd;

    if (FD_ISSET(main_socket, &c_rfd)) {
        serving_socket = accept(main_socket, (struct sockaddr *) &her_addr, &her_addr_size);
        if (serving_socket >= 0) {
            FD_SET(serving_socket, &c_rfd);
        }
    }
    while (1) { ///run forever. city never sleeps.
        for (fd = main_socket + 1; fd < getdtablesize(); fd++)
            if (FD_ISSET(fd, &c_rfd)) {
                return_code = read(fd, &inmsg, 100 * sizeof(char));
                printf("%s\n", inmsg);
                if (!return_code) {
                    close(fd);
                    FD_CLR(fd, &rfd);
                } else if (return_code > 0) {// good read, can write.{
                    strcpy(outmsg, department[rand() % 4]);
                    write(fd, outmsg, 100 * sizeof(char));
                    strcpy(outmsg2, emergency[rand() % 6]);
                    write(fd, outmsg2, 100 * sizeof(char));
                    read(fd, &inmsg, 100 * sizeof(char));
                    printf("%s\n", inmsg);
                    break;
                } else {
                    perror("read() fails");
                    exit(EXIT_FAILURE);
                }
            }
        printf("continue [Y/n]?\n");
        char user_choice;
        scanf(" %c", &user_choice);
        if (tolower(user_choice) == 'n')
            break;
    }
//    time_after=time(NULL);
//    total_time=time_after-time_before;
    sleep(1);
    return (EXIT_SUCCESS);
}

//======================================================================================================================
//this function terminate the program if there is an error in fork
void terminate(char *error_message) {
    perror(error_message);
    exit(EXIT_FAILURE);
}
//======================================================================================================================
