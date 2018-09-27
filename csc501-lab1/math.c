//i#include <stdio.h>
#include <math.h>
//#define RAND_MAX 077777


double pow(double base, int  exponent) {

double powValue;
        if ((int)exponent == 0){
                powValue = 1.0; // base^0 = 1
	return powValue;
        } else if ((int)exponent < 0) {
                int i = 1;
                while(i < (-1 *(int) exponent)) {
                        powValue = powValue / base;
                        i = i + 1;
                }
	return powValue;
        } else {

                int i = 1;
                while (i < (int)exponent) {
                        powValue = powValue * base;
			i = i + 1;
                }
	return powValue;
        }


//return powValue;

}

double log(double base) {
	double logValue = 0;
	int n = 1;
	while (n <= 20) {

		logValue +=  ( pow ( (base-1) , n ) ) * ( ( pow ( -1.0 , ( n+1 ) ) )  / n );
	 	n += 1;
	}

return logValue;

}

double expdev( double lambda) {
//kprintf("Here");
	double expdevValue;

	do {
		//kprintf("checking");
		expdevValue = (double) rand()/RAND_MAX;

		//kprintf("Expdev : %lu RANDMAX : ", expdevValue);
	} while (expdevValue== 0.0);

double returnValue = -log(expdevValue) / lambda;
//kprintf("%d\n", (int) returnValue);
return returnValue;

}



