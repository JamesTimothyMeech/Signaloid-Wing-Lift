[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/JamesTimothyMeech/Signaloid-Wing-Lift#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/JamesTimothyMeech/Signaloid-Wing-Lift#gh-light-mode-only)

# Simple Model of Lift Generation of an Airfoil Using the Signaloid Cloud Platform

## Plan for Calculating Lift Using Experimental Coefficient of Pressure Measurements for NACA 4415 Airfoil
1) Get distribution of measured coefficients of pressure $C_p$ along airfoil and a point value $v_\infty$ for air velocity upstream of airfoil from publically avaliable experimental data [1] described in article [2]. Select NACA 4415 Airfoil under 3-by-5-Ft Wind Tunnel Tests to download article and raw data. Choose data in Run 57 of file N4415c100.txt because the article explicitly states "Steady state results of the NACA 4415 testing at Reynolds number of 1.00 million showed a baseline maximum lift coefficient of 1.35 at 14.3° angle of attack." so we know that the system was in steady state and without any leading edge grit roughness or pitch oscillations applied [2].
2) Load the upper wing surface $C_p$ and lower wing surface $C_p$ distribution along the wing into one uncertain variable each and take $v_\infty = 95.2$ $\mathrm{ft/s}$ as point value.
3) Use the upper and lower wing surface distributions of $C_p$ to calculate the pressure difference across the wing.
4) Use the pressure difference and the surface area of the wing to calculate the lift.

## Assumptions Required for the Plan to Work
1) My interpretation of the experimental data from [1] and [2] is correct.
2) The air flowing over the airfoil has constant density $\rho$ (it cannot be compressed to change density), and has zero viscosity. Also the experiment is conducted at constant air temperature with constant humidity and a fixed elevation.
3) The airfoil has only horizontal surfaces so none of the pressure difference that generates the lift of the airfoil is lost due to the pressure pushing on a non-horizontal surface.

## Calculate Airfoil Differential Pressure Using Coefficient of Pressure 

Let $v$ be the velocity of the air at a specific point in the flow ($\mathrm{m/s}$) and $v_\infty$ be the uniform velocity of the air upstream from the airfoil ($\mathrm{m/s}$).
Let the coefficient of pressure for each point in the flow be a unitless quantity where

$$C_p = \frac{p-p_\infty}{q_\infty}$$

where $q_\infty = \frac{1}{2} \rho v^2_\infty$. We can re-arrange the equation and substitute $\frac{1}{2} \rho v^2_\infty$ for $q_\infty$ to get

$$p = p_\infty + \frac{1}{2} \rho v^2_\infty C_p$$.

Then $\Delta p = p_\mathrm{Lower} - p_\mathrm{Upper} = \frac{1}{2} \rho v^2_\infty (C_p^\mathrm{Lower}$ - $C_p^\mathrm{Upper})$ as $p_\infty$ is present in the expression for both $p_\mathrm{Lower}$ and $p_\mathrm{Upper}$ and therefore cancels out. Once we have calculated $\Delta p$ the lift is 

$$L = \Delta p A.$$

## Results and Checking Lift Calculation Result Using Experimentally Measured Coefficient of Lift

Run 57 in file N4415c100.txt in the dataset [1] contains an experimentally measured value of $C_L = 1.354$ for the coefficient of lift that we can use to check the result of our program. We can calculate the lift from the coefficient of lift and known parameters 

$$L = C_L q_\infty A = 698 \ \mathrm{N}.$$ 

In our initial experiments (branch: uncertain-C_p-hack) we read in $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ from the Run57.txt in the inputs directory and then loaded the $C_p$ values into distributions.
Using the Signaloid Cloud Platform to simply visualize the distribution of discrete $C_p$ values over the length of the wing and then average those values was not a proper use of the capabilities of the Signaloid Cloud Platform.
This naive approach produced a suprisingly reasonable value for the lift of 722 $\mathrm{N}$ with approximately a 3.4 \% error compared to the experimental result. 
This is likely due to the fact that the upper and lower $C_p$ measurments are reasonably uniformly spaced along the wing and therefore naively averaging them with uniform weight does not introduce a large amount of error. 
The table below shows that the mean lift has a negligible dependence on the processor used by the Signaloid Cloud Platform. 
The results for variance, skewness, and kurtosis should be ignored for the table below because loading the upper and lower $C_p$ values into distrubtions makes the uncertainty in the result artificially large.
As the uncertain variables $C_p^\mathrm{Lower}$, $C_p^\mathrm{Upper}$, $v_\infty$, and $\rho$ in the program are indepedent, the results for each processor variant are the same with and without autocorellation tracking. 
This trend is observable for all subsequent experiments.

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift (N) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
|--------------|----------------------|-------------|----------------------|---------------|--------------------------------|----------------------|----------------------|
| C0-XS        | 32                   | 64 MB       | Disabled             | 722.047433    | 351549.855477                  | 158068720.585565     | 408136562203.337219  |
| C0-XS+       | 32                   | 64 MB       | Autocorrelation      | 722.047433    | 351549.855477                  | 158068720.585565     | 408136562203.337219  |
| C0-S         | 64                   | 64 MB       | Disabled             | 722.047654    | 352249.030375                  | 159318029.403001     | 412785784902.068909  | 
| C0-S+        | 64                   | 64 MB       | Autocorrelation      | 722.047654    | 352249.030375                  | 159318029.403001     | 412785784902.068909  |
| C0-M         | 128                  | 256 MB      | Disabled             | 722.047727    | 352473.791058                  | 159696639.298318     | 414339588245.582153  |
| C0-M+        | 128                  | 256 MB      | Autocorrelation      | 722.047727    | 352473.791058                  | 159696639.298318     | 414339588245.582153  |  
| C0-L         | 256                  | 512 MB      | Disabled             | 722.047751    | 352531.040034                  | 159839100.327245     | 414800142985.874634  |
| C0-L+        | 256                  | 512 MB      | Autocorrelation      | 722.047751    | 352531.040034                  | 159839100.327245     | 414800142985.874634  |
| C0-XL        | 512                  | 1 GB        | Disabled             | 722.047759    | 352542.674467                  | 159864706.009107     | 414886807348.927551  | 
| C0-XL+       | 512                  | 1 GB        | Autocorrelation      | 722.047759    | 352542.674467                  | 159864706.009107     | 414886807348.927551  |
| C0-Bypass    | NA                   | 64 MB       | Disabled             | 721.975565    | NA                             | NA                   | NA                   |        
| C0-Reference | 32                   | 64 MB       | Disabled             | 1023.393931   | NA                             | NA                   | NA                   |

We therefore edited the code to perform a weighted averaging of the $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ values based upon their relative position on the airfoil, taking into account that there are 29 valid measurement values for $C_p^\mathrm{Upper}$ and only 27 valid measurement values for $C_p^\mathrm{Lower}$. 
We chose to explore the inclusion of uncertainty in $v_\infty$, the uniform velocity of the air far upstream from the airfoil ($\mathrm{m/s}$) as this value is squared in our analysis, therefore amplifying any uncertainty already present in the variable. 
The table below shows a slightly improved value for lift of 716.8 $\mathrm{N}$ with 2.70 % error. Now we are not treating the $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ values as distributions the variance, skewness, and kurtosis values reflect the uncertainty we introduced in the $v_\infty$, and $\rho$ variables.
We can see non-neglible improvements in the accuracy of the variance, skewness, and kurtosis with the increasing precision of the processor used by the Signaloid Cloud Platform. 

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift (N) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
|--------------|----------------------|-------------|----------------------|---------------|--------------------------------|----------------------|----------------------|
| C0-XS        | 32                   | 64 MB       | Disabled             | 716.835513    | 220.969401                     | 110.173271           | 138338.298592        |
| C0-XS+       | 32                   | 64 MB       | Autocorrelation      | 716.835513    | 220.969401                     | 110.173271           | 138338.298592        |
| C0-S         | 64                   | 64 MB       | Disabled             | 716.835733    | 222.091034                     | 113.959879           | 144171.186810        |
| C0-S+        | 64                   | 64 MB       | Autocorrelation      | 716.835733    | 222.091034                     | 113.959879           | 144171.186810        |
| C0-M         | 128                  | 256 MB      | Disabled             | 716.835805    | 222.462051                     | 116.178221           | 146696.886020        |
| C0-M+        | 128                  | 256 MB      | Autocorrelation      | 716.835805    | 222.462051                     | 116.178221           | 146696.886020        |
| C0-L         | 256                  | 512 MB      | Disabled             | 716.835829    | 222.584264                     | 117.199216           | 147741.437672        |
| C0-L+        | 256                  | 512 MB      | Autocorrelation      | 716.835829    | 222.584264                     | 117.199216           | 147741.437672        |
| C0-XL        | 512                  | 1 GB        | Disabled             | 716.835837    | 222.625219                     | 117.592641           | 148170.839522        |
| C0-XL+       | 512                  | 1 GB        | Autocorrelation      | 716.835837    | 222.625219                     | 117.592641           | 148170.839522        |
| C0-Bypass    | NA                   | 64 MB       | Disabled             | 716.764165    | NA                             | NA                   | NA                   |
| C0-Reference | 32                   | 64 MB       | Disabled             | 683.976929    | NA                             | NA                   | NA                   |

To address the question of what would happen if we used fewer pitot tubes and therefore got fewer measurements for $C_p$ we wrote a program (branch: two-measurements-only) where we drop all but one datapoint for both $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$.
The table of results below show that this has a far worse effect upon the mean lift prediction of the program. 
The predicted value for lift is approximately 1166 $\mathrm{N}$ and this is approximately a 67 % error when compared to the lift value calculated from the experimentally measured lift coefficient. 
This indicates that making the assumptions we have made and using the data avaliable from [1] the agreement of our calculations with the experimentally measured lift coefficient are limited by the absence of more $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ measurements along the airfoil. 
The agreement of the lift calculated by our program and the lift calculated from the experimentaly-measured lift coefficient is limited by epistemic uncertainty due to the lack of more $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ measurements.
As before we can see non-neglible improvements in the accuracy of the variance, skewness, and kurtosis with the increasing precision of the processor used by the Signaloid Cloud Platform. 
These improvements are neglible compated to the epistemic uncertainty that we have exposed ourselves to by failing to complete more $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ measurements by installing more pitot tubes or alternative pressure measurement sensors. 

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift (N) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
|--------------|----------------------|-------------|----------------------|---------------|--------------------------------|----------------------|----------------------|
| C0-XS        | 32                   | 64 MB       | Disabled             | 1165.632326   | 584.273621                     | 473.698986           | 967187.578964        | 
| C0-XS+       | 32                   | 64 MB       | Autocorrelation      | 1165.632326   | 584.273621                     | 473.698986           | 967187.578964        |
| C0-S         | 64                   | 64 MB       | Disabled             | 1165.632683   | 587.239373                     | 489.979815           | 1007968.021484       |
| C0-S+        | 64                   | 64 MB       | Autocorrelation      | 1165.632683   | 587.239373                     | 489.979815           | 1007968.021484       |
| C0-M         | 128                  | 256 MB      | Disabled             | 1165.632801   | 588.220393                     | 499.517758           | 1025626.362879       |
| C0-M+        | 128                  | 256 MB      | Autocorrelation      | 1165.632801   | 588.220393                     | 499.517758           | 1025626.362879       |
| C0-L         | 256                  | 512 MB      | Disabled             | 1165.632839   | 588.543542                     | 503.907609           | 1032929.310753       |
| C0-L+        | 256                  | 512 MB      | Autocorrelation      | 1165.632839   | 588.543542                     | 503.907609           | 1032929.310753       |
| C0-XL        | 512                  | 1 GB        | Disabled             | 1165.632852   | 588.651833                     | 505.599172           | 1035931.459392       |
| C0-XL+       | 512                  | 1 GB        | Autocorrelation      | 1165.632852   | 588.651833                     | 505.599172           | 1035931.459392       |
| C0-Bypass    | NA                   | 64 MB       | Disabled             | 1165.516307   | NA                             | NA                   | NA                   |
| C0-Reference | 32                   | 64 MB       | Disabled             | 1175.779921   | NA                             | NA                   | NA                   |

## References

[1] https://www.nrel.gov/wind/nwtc/airfoils-osu-data.html

[2] Hoffmann, M. J., R. Reuss Ramsay, and G. M. Gregorek. Effects of grit roughness and pitch oscillations on the NACA 4415 airfoil. No. NREL/TP-442-7815. National Renewable Energy Lab.(NREL), Golden, CO (United States); The Ohio State Univ., Columbus, OH (United States), 1996.
