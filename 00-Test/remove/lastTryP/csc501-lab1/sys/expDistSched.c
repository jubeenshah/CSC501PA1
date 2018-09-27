#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "math.h"
#include "sched.h"

extern int ctxsw(int,int,int,int);

int expDistSched() {

	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	optr = &proctab[currpid];

	int priorityCalc = (int) expdev(0.1)

}

