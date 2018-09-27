#define EXPDISTSCHED 1 
#define LINUXSCHED 2
extern int SCHEDULER; 
extern void setschedclass (int sched_class);
extern int getschedclass();
extern int expdistsched();
extern int linuxsched();
