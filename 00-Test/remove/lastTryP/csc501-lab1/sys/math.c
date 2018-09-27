#include <math.h>
double pow(double x, int y){
	if(x<=0.000001)
		return x;
        double result=1;
	int i;
        for(i=1;i<=y;i++)
                result=result*x;
        return result;
}

double log(double x){
        double y=1-x;
	int i;
        //x=1-y
        //log(1-y)=-y-(y^2/2)-(y^3/3)-....20terms... From Taylor series
        double result=0;
        for(i=1;i<=20;i++){
	        result+= pow(y,i)/i;
	
        }
        return -1*result;
}

double expdev(double lambda) {
	double dummy;
	do
		dummy=(double) rand() / RAND_MAX;
	while (dummy == 0.0);
        //kprintf("dummy %d\n", (int) dummy);
	return -log(dummy) / lambda;
}
