/* linuxsched.c  -  linuxsched */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <proc.h>
#include <sched.h>

extern int ctxsw(int, int, int, int);
extern int quantum[NPROC];
extern unsigned long ctr1000;

/*-----------------------------------------------------------------------
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
extern int epochtime;
extern int starttime;
void initepoch(){
	int i;
	struct pentry   *proc = proctab;
	epochtime=0;
	for(i=1;i<NPROC;i++){
		if(proc[i].pstate==PRFREE){
			quantum[i]=0;
			continue;
		}
		quantum[i]=(quantum[i]/2)+proc[i].pprio;
		epochtime+=quantum[i];
	}
	starttime=ctr1000;
	//kprintf("\nnew epoch time: %d\n",epochtime);
	
	
}
int linuxsched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	struct pentry   *proc = proctab;
	/* no switch needed if current process priority higher than next*/
	optr= &proctab[currpid];
	if(optr->pstate == PRFREE){
                quantum[currpid]=0;
                //kprintf("died soon %d ",preempt);
                epochtime-=preempt;
        }
        else if(currpid>0){
		//kprintf("preempt:%d PID: %d ",preempt,currpid);
		quantum[currpid]=preempt;
	}
		
        if(ctr1000-starttime>=epochtime)
                initepoch();

	int i;
	int newpid=0,maxgoodness=0;
	for(i=1;i<NPROC;i++)
	{
		if(quantum[i]>maxgoodness && (proc[i].pstate==PRREADY || proc[i].pstate==PRCURR)){
			maxgoodness=quantum[i];
			newpid=i;
		}
	}
	nptr= &proctab[newpid];
	/* force context switch */
	if(optr->pstate == PRCURR && newpid==currpid){
                if(newpid ==0 && epochtime>0)
                        preempt=epochtime-(ctr1000-starttime);
                else if(newpid==0)
#ifdef  RTCLOCK
                        preempt=QUANTUM;
#endif
                else
                        preempt = quantum[newpid];
		//kprintf("Sched NP: %d Q: %d ",newpid,preempt);
		return OK;
	}
	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = newpid) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/

	if(newpid==0 && epochtime>0)
		preempt = epochtime-(ctr1000-starttime);
	else if(newpid==0)
#ifdef  RTCLOCK
		preempt = QUANTUM;
#endif
	else
		preempt = quantum[newpid];		/* reset preemption counter	*/	
	 //kprintf("Sched NP: %d Q: %d ",newpid,preempt);	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}
