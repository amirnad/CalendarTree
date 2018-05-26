#ifndef _CALENDAREVENT_H_
#define _CALENDAREVENT_H_

#include <iostream>

using namespace std;

class CalendarEvent
{
	time_t m_StartTime; // time_t is defined in the header file <ctime>
	time_t m_Duration; // duration of the event, in seconds
	string m_Description; // should not contain special characters or newline
public:
	// constructor, destructor, other methods as needed
	CalendarEvent(time_t i_StartTime, time_t i_Duration, string i_Description) 
		: m_StartTime(i_StartTime), m_Duration(i_Duration), m_Description(i_Description) {}
	time_t getStartTime() { return m_StartTime; }
	time_t getDuration() { return m_Duration; }
	string getDescription() { return m_Description; }
	void print(); // "print" the event to cout
	bool isEventStillHappening(time_t i_RequestedTime);
};

#endif