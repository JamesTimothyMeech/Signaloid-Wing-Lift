#include <stdio.h>
#include <uxhw.h>
#include <math.h>

enum
{
	kMaxData = 29,
};

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
	double	v_infinity, C_p_upper, v_upper, C_p_lower, v_lower, rho, lift, A, upper_x_c, lower_x_c;

	char buffer[1024];
	double C_p_upper_a[kMaxData];
	double C_p_lower_a[kMaxData];
	FILE *file_pointer;
	file_pointer = fopen("Run57.txt", "r+");
	if (file_pointer == NULL)
	{
		printf("Could not open %s input file!\n", "Run57.txt");
		return 0;
	}

	for(int i = 0 ; i < 7 ; i++)
	{
		fgets(buffer, sizeof(buffer), file_pointer);
	}

	for(int i = 0 ; i < 29 ; i++)
	{
		fscanf(file_pointer, "%lf, %lf, %lf, %lf\n", &upper_x_c, &C_p_upper, &lower_x_c, &C_p_lower);
		C_p_upper_a[i] = C_p_upper;
		C_p_lower_a[i] = C_p_lower;
	}

	rho = 1.225;
	A = 1.0;
	v_infinity = 95.2 * 0.3048;
	printf("v_infinity = %lf\n", v_infinity);

	C_p_lower = UxHwDoubleDistFromSamples(C_p_lower_a, kMaxData - 25);
	printf("C_p_lower = %lf\n", C_p_lower);

	C_p_upper = UxHwDoubleDistFromSamples(C_p_upper_a, kMaxData);
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

