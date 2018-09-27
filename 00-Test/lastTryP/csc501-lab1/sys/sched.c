#include "sched.h"

int scheduledClass;

void setschedclass (int classOfScheduler) {
	
	scheduledClass = classOfScheduler;

}

void getschedclass() {
	return scheduledClass;
}
