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

	if ((startTime + duration >= requestedTime) && (requestedTime + duration > startTime))
	{//there is already an event on this time
		isHappening = true;
	}
	else
	{//there is no event on this time
		isHappening = false;
	}

	return isHappening;
}
