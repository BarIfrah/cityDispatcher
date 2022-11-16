To run, run main with a port.
CityUtility runs with ip(no default. I chose localhost) and same port from main
example - ./CityUtility 127.0.0.1 5555
          ./main 5555

Main process will wait for clients to log on to the socket (up to 5 clients, can change - it's hard coded).

main sends to handle 2 messages - one with the handling force and the other with emergency title.

Handler deals with the emergency and notifies its own terminal.

Meant to create a linked list of nodes containing struct instances of a 'case', but had no time.

Tasks are randomised in main, created as a struct and sent to the socket for the "CityUtility" process to continue.
Did not have enough time to submit task as requested. Missions are assigned randomly by a static array of
missions and utilities.

socket works good and the IPC is happy :)

