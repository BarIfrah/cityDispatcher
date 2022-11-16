#include "EventCreator.h"
#include <string.h>
struct Event createEvent(uint8_t handlingDept, uint8_t severity, const char *title, const char *desc){
    Event event = {handlingDept, severity};
    strcpy(event.eventTitle,  title);
    strcpy(event.eventDescription, desc);
    return event;
}