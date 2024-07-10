#include <stdio.h>
#include <uxhw.h>
#include <math.h>

int
main(int argc, char *  argv[])
{
	double	v_infinity, C_p, v;

	v_infinity = UxHwDoubleGaussDist(95.2 * 0.3048, (95.2 * 0.3048) / 100 );
	printf("v_infinity = %lf\n", v_infinity);

	C_p = UxHwDoubleUniformDist(-0.300, 0.992);
	printf("C_p = %lf\n", C_p);

	v = v_infinity*sqrt(1 - C_p);
	printf("v = %lf\n", v);

#ifdef DEBUG
	printf("debug message\n");
#endif

	return 0;
}

