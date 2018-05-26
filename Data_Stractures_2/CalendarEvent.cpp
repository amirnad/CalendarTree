#include "CalendarEvent.h"
#include <string>

void CalendarEvent::print()
{
	cout << m_StartTime << " " << m_Duration << m_Description << endl;
}

//Getting a time and checking if the even is happening on this time
bool CalendarEvent::isEventStillHappening(time_t i_RequestedTime)
{
	bool isHappening = false;

	time_t currEventEndTime = m_StartTime + m_Duration;

	if ((currEventEndTime > i_RequestedTime) && (m_StartTime <= i_RequestedTime))
	{//There is already an event on this time
		isHappening = true;
	}

	return isHappening;
}


