#include <stdio.h>
#include <uxhw.h>
#include <math.h>

double 
calculate_velocity_from_C_p(double C_p, double v_infinity)
{
	return v_infinity*sqrt(1 - C_p);
}

double
calculate_lift_from_velocities(double v_lower, double v_upper, double rho, double A)
{
	return 0.5 * rho * A * (pow(v_upper, 2) - pow(v_lower,2));
}

int
main(int argc, char *  argv[])
{
	double	v_infinity, C_p_upper, v_upper, C_p_lower, v_lower, rho, lift, A;

	rho = 1.225;
	A = 1.0;

	v_infinity = UxHwDoubleGaussDist(95.2 * 0.3048, (95.2 * 0.3048) / 100 );
	printf("v_infinity = %lf\n", v_infinity);

	C_p_lower = UxHwDoubleUniformDist(-0.300, 0.992);
	printf("C_p_lower = %lf\n", C_p_lower);

	C_p_upper = UxHwDoubleUniformDist(-3.954, -0.300);
	printf("C_p_upper = %lf\n", C_p_upper);

	v_lower = calculate_velocity_from_C_p(C_p_lower, v_infinity);
	printf("v_lower = %lf\n", v_lower);
	v_upper = calculate_velocity_from_C_p(C_p_upper, v_infinity);
	printf("v_upper = %lf\n", v_upper);

	lift = calculate_lift_from_velocities(v_lower, v_upper, rho, A);
	printf("lift = %lf\n", lift);

#ifdef DEBUG
	printf("debug message\n");
#endif

	return 0;
}

