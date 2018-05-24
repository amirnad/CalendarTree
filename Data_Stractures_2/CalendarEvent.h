#ifndef _CALENDAREVENT_H_
#define _CALENDAREVENT_H_

#include <iostream>
#include <list>

using namespace std;

class CalendarEvent
{
	time_t startTime; // time_t is defined in the header file <ctime>
	time_t duration; // duration of the event, in seconds
	string description; // should not contain special characters or newline
public:
	// constructor, destructor, other methods as needed
	CalendarEvent(time_t _startTime, time_t _duration, string _description) 
		: startTime(_startTime), duration(_duration), description(_description) {}
	time_t getStartTime() { return startTime; }
	time_t getDuration() { return duration; }
	string getDescription() { return description; }
	void print(); // "print" the event to cout
	bool isEventStillHappening(time_t requestedTime);
	bool isEventInRange(time_t requestedTime);
	bool isWithinBounds(time_t newEventStart, time_t newEventEnd);

};

#endif