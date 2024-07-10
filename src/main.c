#include <stdio.h>
#include <uxhw.h>
#include <math.h>

enum
{
	kMaxData = 29,
};

double
calculate_lift_from_C_p(double C_p_upper, double C_p_lower, double v_infinity, double rho, double A)
{
	return 0.5 * rho * pow(v_infinity, 2) * (C_p_lower - C_p_upper);
}

int
main(int argc, char *  argv[])
{
	double	v_infinity, C_p_upper, C_p_lower, rho, lift, A, upper_x_c, lower_x_c;

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

	C_p_lower = UxHwDoubleDistFromSamples(C_p_lower_a, kMaxData - 2);
	printf("C_p_lower = %lf\n", C_p_lower);

	C_p_upper = UxHwDoubleDistFromSamples(C_p_upper_a, kMaxData);
	printf("C_p_upper = %lf\n", C_p_upper);

	lift = calculate_lift_from_C_p(C_p_upper, C_p_lower, v_infinity, rho, A);
	printf("lift = %lf\n", lift);

#ifdef DEBUG
	printf("debug message\n");
#endif

	return 0;
}

