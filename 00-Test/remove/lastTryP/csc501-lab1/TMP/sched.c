#include <sched.h>
#include <kernel.h>
#include <proc.h>
int SCHEDULER=0; //default scheduler is runs process with max priority
int quantum[NPROC];
int epochtime;
int starttime;
extern void initepoch();
extern unsigned long ctr1000;

void setschedclass (int sched_class){
	SCHEDULER=sched_class;
	if(SCHEDULER==LINUXSCHED){
		int i;
		for(i=0;i<NPROC;i++)
			quantum[i]=0;
		epochtime=preempt;
		starttime=ctr1000;
	}
}
int getschedclass(){
	return SCHEDULER;
}
