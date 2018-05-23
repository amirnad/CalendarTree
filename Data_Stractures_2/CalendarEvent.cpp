#include "CalendarEvent.h"
#include <string>

void CalendarEvent::print()
{
	cout << startTime << " " << duration << description << endl;
}

bool CalendarEvent::isEventStillHappening(time_t requestedTime)
{
	bool isHappening = false;
	time_t currEventEndTime = startTime + duration;

	if (currEventEndTime > requestedTime && startTime <= requestedTime)
	{//There is already an event on this time
		isHappening = true;
	}

	return isHappening;
}

bool CalendarEvent::isEventInRange(time_t requestedTime)
{
	bool res = false;
	time_t range = startTime + duration;
	if (range > requestedTime)
	{
		res = true;
	}
	return res;
}

bool CalendarEvent::isWithinBounds(time_t newEventStart, time_t newEventEnd)
{
	bool res = false;
	time_t fullTime = startTime + duration;

	if (newEventStart <= fullTime && startTime < newEventEnd)
	{
		res = true;
	}
	return res;
}
