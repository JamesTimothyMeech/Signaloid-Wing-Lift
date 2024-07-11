#include <stdio.h>
#include <uxhw.h>
#include <math.h>

enum
{
	kMaxData = 29,
};

double
calculate_lift_from_C_p(double C_p_upper, double C_p_lower, double v_infinity, double rho, double surface_area)
{
	return 0.5 * rho * pow(v_infinity, 2) * (C_p_lower - C_p_upper) * surface_area;
}

int
main(int argc, char *  argv[])
{
	const double krho = 1.225;
	const double ksurface_area = 1.0;
	const double kconvert_ft_per_s_to_m_per_s = 0.3048;
	const double kv_infinity_ft_per_s = 95.2;
	double	v_infinity, v_infinity_mean, v_infinity_standard_deviation, C_p_upper_average, C_p_lower_average, lift, rho;

	char buffer[1024];
	double C_p_upper_a[kMaxData];
	double C_p_lower_a[kMaxData];
	double upper_x_c_a[kMaxData];
	double lower_x_c_a[kMaxData];

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
		fscanf(file_pointer, "%lf, %lf, %lf, %lf\n", &upper_x_c_a[i], &C_p_upper_a[i], &lower_x_c_a[i], &C_p_lower_a[i]);
	}
	
	v_infinity_mean = kv_infinity_ft_per_s * kconvert_ft_per_s_to_m_per_s;
	v_infinity_standard_deviation = 0.01 * v_infinity_mean;
	v_infinity = UxHwDoubleGaussDist(v_infinity_mean, v_infinity_standard_deviation); 
	printf("v_infinity = %lf\n", v_infinity);

	rho = UxHwDoubleUniformDist(krho - (krho * 0.01), krho + (krho * 0.01)); 
	printf("rho = %lf\n", rho);
	

	C_p_upper_average = 0;
	C_p_lower_average = 0;
	

	for(int i = 0 ; i < kMaxData - 1 ; i++)
	{
		C_p_upper_average = C_p_upper_average + 0.5 * (C_p_upper_a[i] + C_p_upper_a[i+1]) * (upper_x_c_a[i+1] - upper_x_c_a[i]);
		// We need to do this because there are only 27 valid C_p readings for the coefficient of pressure below the airfoil
		if (i < 26)
		{
			C_p_lower_average = C_p_lower_average + 0.5 * (C_p_lower_a[i] + C_p_lower_a[i+1]) * (lower_x_c_a[i+1] - lower_x_c_a[i]);
		}
	}

	printf("C_p_upper_average = %lf\n", C_p_upper_average);
	printf("C_p_lower_average = %lf\n", C_p_lower_average);

	lift = calculate_lift_from_C_p(C_p_upper_average, C_p_lower_average, v_infinity, rho, ksurface_area);

	printf("lift mean = %lf\n", UxHwDoubleNthMode(lift, 1));
	printf("lift standard deviation = %lf\n", UxHwDoubleNthMode(lift, 2));
	printf("lift skewness = %lf\n", UxHwDoubleNthMode(lift, 3));
	printf("lift kurtosis = %lf\n", UxHwDoubleNthMode(lift, 4));

#ifdef DEBUG
	printf("debug message\n");
#endif

	return 0;
}

