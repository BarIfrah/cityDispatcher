#include "EventCreator.h"

struct Event createEvent(uint8_t handlingDept, uint8_t severity, char *title, char *desc){
    Event event = {handlingDept, severity};
    event.eventTitle = title;
    event.eventDescription = desc;
    return event;
}