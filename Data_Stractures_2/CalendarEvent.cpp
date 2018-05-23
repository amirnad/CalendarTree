#include "CalendarEvent.h"
#include <string>

void CalendarEvent::print()
{
	cout << "Start Time: " << startTime << endl;
	cout << "Duration: " << duration << endl;
	cout << "Description: " << description << endl;
}

bool CalendarEvent::isEventStillHappening(time_t requestedTime)
{
	bool isHappening = false;
	time_t currEventEndTime = startTime + duration;

	if (requestedTime >= startTime && requestedTime <= currEventEndTime)
	{//there is already an event on this time
		isHappening = true;
	}
	else
	{//there is no event on this time
		isHappening = false;
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
