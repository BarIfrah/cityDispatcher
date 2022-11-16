#include <stdlib.h>


typedef struct Event{
    uint8_t department;
    uint8_t severity;
    char eventTitle[100];
    char eventDescription[100];
}Event;

struct Event createEvent(uint8_t, uint8_t, const char*, const char*);