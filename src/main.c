#include <stdio.h>
#include <uxhw.h>
#include <math.h>

// Create a constant for the number of elements in the arrays that we store the C_p values in
enum
{
	kMaxData = 29,
};

// Define an function to calculate the equation we chose for calculating lift in the readme
double
calculate_lift_from_C_p(double C_p_upper, double C_p_lower, double v_infinity, double rho, double surface_area)
{
	return 0.5 * rho * pow(v_infinity, 2) * (C_p_lower - C_p_upper) * surface_area;
}

int
main(int argc, char *  argv[])
{

	// Define all constant variables used in the program
	const double krho = 1.225;
	const double ksurface_area = 1.0;
	const double kconvert_ft_per_s_to_m_per_s = 0.3048;
	const double kv_infinity_ft_per_s = 95.2;

	// Define all non-constant variables used in the program
	double	v_infinity, v_infinity_mean, v_infinity_standard_deviation, C_p_upper_average, C_p_lower_average, lift, rho;
	char buffer[1024];

	// Define arrays to store upper and lower C_p values for the airfoil and the x coordinate of the C_p measurement normalized by the airfoil chord length
	double C_p_upper_a[kMaxData];
	double C_p_lower_a[kMaxData];
	double upper_x_c_a[kMaxData];
	double lower_x_c_a[kMaxData];

	// Attempt to open the Run57.txt file in the inputs directory and print an error message if there is a problem 
	FILE *file_pointer;
	file_pointer = fopen("Run57.txt", "r+");
	if (file_pointer == NULL)
	{
		printf("Could not open %s input file!\n", "Run57.txt");
		return 0;
	}

	// Move the file pointer through the first text sections of the text file that we do not care about	
	for(int i = 0 ; i < 7 ; i++)
	{
		fgets(buffer, sizeof(buffer), file_pointer);
	}

	// Read the 29 values for upper and lower C_p values and their normalized x coordinate values into arrays
	// Note that there are only 27 valid values for lower C_p and the corresponding normalized x coordinate so we have to deal with that later
	for(int i = 0 ; i < 29 ; i++)
	{
		fscanf(file_pointer, "%lf, %lf, %lf, %lf\n", &upper_x_c_a[i], &C_p_upper_a[i], &lower_x_c_a[i], &C_p_lower_a[i]);
	}
	
	// Convert the air velocity upstream of the airfoil from ft/s to m/s
	v_infinity_mean = kv_infinity_ft_per_s * kconvert_ft_per_s_to_m_per_s;

	// Create a Gaussian uncertain variable where the mean is the point value velocity in m/s and the standard deviation is 1% of that value
	// Note that the Gaussian is defined using the standard deviation but when querying the moments we get back the variance instead
	v_infinity_standard_deviation = 0.01 * v_infinity_mean;
	v_infinity = UxHwDoubleGaussDist(v_infinity_mean, v_infinity_standard_deviation); 
	printf("v_infinity = %lf\n", v_infinity);

	// Create a uniform uncertain variable for the air density where the range is +/- 1% of the constant point value we chose for air density
	rho = UxHwDoubleUniformDist(krho - (krho * 0.01), krho + (krho * 0.01)); 
	printf("rho = %lf\n", rho);
	
	// Set the variables that we use to hold the upper and lower C_p averages to zero so we can build up the average using summation
	C_p_upper_average = 0;
	C_p_lower_average = 0;
	
	// Sum up the upper and lower C_p values seperately using their coordinates to get a weighted average over each section of airfoil
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

	printf("lift = %lf\n", lift);
	printf("lift mean = %lf\n", UxHwDoubleNthMoment(lift, 1));
	printf("lift variance = %lf\n", UxHwDoubleNthMoment(lift, 2));
	printf("lift third centralized moment = %lf\n", UxHwDoubleNthMoment(lift, 3));
	printf("lift fourth centralized moment = %lf\n", UxHwDoubleNthMoment(lift, 4));

#ifdef DEBUG
	printf("debug message\n");
#endif

	return 0;
}

