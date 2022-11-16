#include <stdlib.h>


typedef struct Event{
    uint8_t department;
    uint8_t severity;
    char *eventTitle;
    char *eventDescription;
}Event;

struct Event createEvent(uint8_t, uint8_t, char *, char *);