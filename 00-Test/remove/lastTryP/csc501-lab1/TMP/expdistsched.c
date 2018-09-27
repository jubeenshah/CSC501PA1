/* expdistsched.c  -  expdistsched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
#include <sched.h>

extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * expdistsched  --  reschedule processor based on Exponential Distribution Scheduler
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int expdistsched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	double expprio=expdev(0.1);
	int next = q[rdyhead].qnext;
	int newpid;
	if(lastkey(rdytail)==MININT){  /* queue is empty */
		preempt=QUANTUM;
		return(OK);
	}
	if(expprio>=lastkey(rdytail))
		newpid = q[(rdytail)].qprev;//getlast(rdytail);
	else{
		while (q[next].qkey <= expprio)
			next = q[next].qnext;
		
		/*Round Robin : we want to schedule the last out of the process with same priorities in the queue as we insert a process with priority x just before the first occurrence of process with priority x*/
		while(q[next].qkey == q[q[next].qnext].qkey)
			next = q[next].qnext;
		newpid = next;//dequeue(next);
	}

        /* no switch needed if current process priority next higher value than random number or random number is greater than the largest(currpid) if priorities of all processes*/
	optr= &proctab[currpid];
	if(optr->pstate == PRCURR)
		if((optr->pprio < q[newpid].qkey && expprio < optr->pprio) || (optr->pprio > q[newpid].qkey && expprio > optr->pprio)){
			preempt = QUANTUM;
	        	return(OK);
		}

	dequeue(newpid);
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	nptr = &proctab[ (currpid = newpid) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	/* The OLD process returns here when resumed. */
	return OK;
}
