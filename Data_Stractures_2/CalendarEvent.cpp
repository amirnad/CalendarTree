#include "CalendarEvent.h"
#include <string>

void CalendarEvent::print()
{
	cout << startTime << " " << duration << description << endl;
}

//Getting a time and checking if the even is happening on this time
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


