#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sched.h>
#include <math.h>

#define  SETONE 	1
#define  SETHALF	0.5
#define  SETZERO	0

unsigned long currSP;
extern int ctxsw(int, int, int, int);

int resched()
{
	register struct	pentry	*optr;
	register struct	pentry	*nptr;
	int i,newEpoch,nextProcess,maximumGoodness,exponentialSchedPriority;
	optr= &proctab[currpid];
	int classToBeScheduledCheck = getschedclass();

	switch (classToBeScheduledCheck) {
		case EXPDISTSCHED:
		exponentialSchedPriority=expdev(0.1);
				 if (optr->pstate == PRCURR){
					optr->pstate = PRREADY;
					insert(currpid,rdyhead,optr->pprio);
				}
				nextProcess = q[rdyhead].qnext;
					while (q[nextProcess].qkey <(int) exponentialSchedPriority){
					nextProcess = q[nextProcess].qnext;
				}
				if(nextProcess>NPROC) {
				nextProcess=q[rdytail].qprev;
			}
					currpid = nextProcess;
					nptr = &proctab[currpid];
					nptr->pstate = PRCURR;
					dequeue(nextProcess);
					#ifdef	RTCLOCK
								preempt = QUANTUM;
					#endif
				break;

			case LINUXSCHED:
			newEpoch=SETONE;
			proctab[currpid].quantum=preempt;
			if(proctab[currpid].quantum == SETZERO){
					proctab[currpid].goodness=SETZERO;
			}
			else {
					proctab[currpid].goodness=proctab[currpid].pprio+proctab[currpid].quantum;
			}

			i = SETZERO;
			do {
				if( ( proctab[i].pstate==PRREADY || proctab[i].pstate==PRCURR ) && proctab[i].quantum!=SETZERO) {
					newEpoch = SETZERO;
				}
				i = i + SETONE;
			} while (i < NPROC);

			if(newEpoch==SETONE){
				i = SETZERO;
				do {
					if(proctab[i].pstate!=PRFREE) {	
							proctab[i].quantum=proctab[i].pprio+(int)(SETHALF*proctab[i].quantum);
							proctab[i].goodness=proctab[i].pprio;
					}
					i = i + SETONE;

				} while(i < NPROC);
			}

			if (optr->pstate == PRCURR) {
				optr->pstate = PRREADY;
				insert(currpid,rdyhead,optr->pprio);
			}
			maximumGoodness = SETZERO;
			i = SETZERO;
			do {
				if((proctab[i].pstate==PRREADY || proctab[i].pstate==PRCURR) && proctab[i].goodness>=maximumGoodness && proctab[i].quantum!=SETZERO){
						maximumGoodness=proctab[i].goodness;
						nextProcess=i;
					}
				i = i + SETONE;
			} while(i < NPROC);
			currpid = dequeue(nextProcess);
			nptr=&proctab[currpid];
			nptr->pstate = PRCURR;
			preempt = nptr->quantum;
			break;

			default:
					optr= &proctab[currpid];
					
					if ( ( optr->pstate == PRCURR) &&
					   (lastkey(rdytail)<optr->pprio)) {
						return(OK);
					}
					if (optr->pstate == PRCURR) {
						optr->pstate = PRREADY;
						insert(currpid,rdyhead,optr->pprio);
					}
					currpid = getlast(rdytail);
					nptr = &proctab[ currpid ];
					nptr->pstate = PRCURR;
				#ifdef	RTCLOCK
				preempt = QUANTUM;
				#endif
				break;
			}

			ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;


	}
