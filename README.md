[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/JamesTimothyMeech/Signaloid-Wing-Lift#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/JamesTimothyMeech/Signaloid-Wing-Lift#gh-light-mode-only)

# Lift Generation of an Airfoil Using the Signaloid Cloud Platform
This repository contains a program modeling the lift generated by a NACA 4415 airfoil and the associated uncertainties using the Signaloid Cloud Platform. We rely on data experimentally measured using a section of NACA 4415 airfoil instrumented with sensors inside a wind tunnel.
The image below is the side profile of the NACA 4415 airfoil used to collect the experimental data from [1] and pictured in [2]. 

![airfoil](https://github.com/user-attachments/assets/b991dc39-6738-41c2-a854-5b3af40ba36e)

## Plan for Calculating Lift Using Experimental Data for NACA 4415 Airfoil
1) Obtain a set of measured coefficients of pressure $C_p$ (a.u.) along the airfoil and a point value $v_\infty$ for air velocity ($\mathrm{m/s}$) upstream of airfoil from publically avaliable experimental data [1] described in article [2]. Select NACA 4415 Airfoil under 3-by-5-Ft Wind Tunnel Tests to download the article and associated raw experimental data. Choose data in Run 57 of file N4415c100.txt because the article explicitly states "Steady state results of the NACA 4415 testing at Reynolds number of 1.00 million showed a baseline maximum lift coefficient of 1.35 at 14.3° angle of attack." so we know that the system was in steady state and without any leading edge grit roughness or pitch oscillations applied [2].
2) Load the upper airfoil surface $C_p$ and lower airfoil surface $C_p$ distribution along the airfoil into one uncertain variable each and take $v_\infty = 95.2$ $\mathrm{ft/s}$ as point value.
3) Use the upper and lower airfoil surface distributions of $C_p$ to calculate the pressure difference across the airfoil.
4) Use the pressure difference and the surface area of the airfoil to calculate the lift.

## Assumptions Required for the Plan to Work
1) My interpretation of the raw experimental data from source [1] and article [2] is correct.
2) The fluid is air and the air flowing over the airfoil has constant density $\rho$ ($\mathrm{kg/m^3}$), and has zero viscosity. Also the experiment is conducted at constant air temperature with constant humidity and a fixed elevation.
3) The airfoil has only horizontal surfaces so none of the pressure difference that generates the lift force becomes a component in the horizontal direction.

## Calculate Airfoil Differential Pressure Using Coefficient of Pressure 

As we have experimental measurements of the coefficient of pressure we do not need to Bernoulli's equation to calculate the lift produced by the airfoil [3].
Let $v$ be the velocity of the air ($\mathrm{m/s}$) at a specific point in the flow and $v_\infty$ be the uniform velocity ($\mathrm{m/s}$) of the air upstream from the airfoil.
Let the coefficient of pressure for each point in the flow be a unitless quantity where

$$C_p = \frac{p-p_\infty}{q_\infty}$$

where $q_\infty = \frac{1}{2} \rho v^2_\infty$. We can re-arrange the equation and substitute $\frac{1}{2} \rho v^2_\infty$ for $q_\infty$ to obtain

$$p = p_\infty + \frac{1}{2} \rho v^2_\infty C_p$$.

Let $\Delta p$ be the pressure difference across the wing ($\mathrm{kg/ms^2}$) and $p_\mathrm{Lower}$ and $p_\mathrm{Upper}$ be the pressure above and below the airfoil respectively ($\mathrm{kg/ms^2}$). Let $A$ be the surface area of the underside of the airfoil ($\mathrm{m^2}$).
Then $\Delta p = p_\mathrm{Lower} - p_\mathrm{Upper} = \frac{1}{2} \rho v^2_\infty (C_p^\mathrm{Lower}$ - $C_p^\mathrm{Upper})$ as $p_\infty$ is present in the expression for both $p_\mathrm{Lower}$ and $p_\mathrm{Upper}$ and therefore cancels out. Once we have calculated $\Delta p$ the lift is 

$$L = \Delta p A.$$

We chose to explore the inclusion of uncertainty in $v_\infty$, the uniform velocity of the air far upstream from the airfoil ($\mathrm{m/s}$) as this value is squared in our analysis, therefore amplifying any uncertainty already present in the variable. 
In all of our experiments we used a Gaussian distribution to model the uncertainty in $v_\infty$ and a uniform distribution to model the uncertainty in $\rho$.
For $v_\infty$ we used a mean of $v_\infty = 95.2$ $\mathrm{ft/s}$ converted to have units $\mathrm{m/s}$ with a standard deviation of 1 % of the mean. 
For $\rho$ we used a range of 1.225 $\mathrm{kg/m^3}$ +/- 1 % of the point value. 

## Results and Checking Lift Calculation Using Experimentally Measured Coefficient of Lift

Run 57 in file N4415c100.txt in the dataset [1] contains an experimentally measured value of $C_L = 1.354$ (a.u.) for the coefficient of lift that we can use to check the result of our program. We can calculate the lift from the coefficient of lift and known parameters 

$$L = C_L q_\infty A = 698 \ \mathrm{N}.$$ 

In our initial experiments (branch: uncertain-C_p-hack) the program reads in the measurements of $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ from the Run57.txt file in the inputs directory and then loaded the $C_p$ values into distributions.
Although we refer to pressure measurements as $C_p$ measurements please note that in the experimental setup used by the authors of [2] to collect the data [1] each $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ value corresponds to a physical pressure sensor in their setup.
The authors of [2] refer to these pressure sensors as PSI Pressure Sensing Modules and do not specify whether or not these devices are pitot tubes. A Google search did not provide any additional information. The plots below show the distribution of upper and lower $C_p$ values ignoring the spatial position of the $C_p$ values for the uncertain-C_p-hack branch on a C0-XL core.

<img style="text-align:center" width="344" alt="C_p_lower" src="https://github.com/user-attachments/assets/abde78b6-d5bf-4d0d-85de-f8fd983fef65">
<img style="text-align:center" width="342" alt="C_p_upper" src="https://github.com/user-attachments/assets/7a3bf5bb-6330-4350-97d3-3b544a87828e">

Using the Signaloid Cloud Platform to visualize the distribution of discrete $C_p$ values over the length of the airfoil and then average those values was not a proper use of the capabilities of the Signaloid Cloud Platform.
Even so, this naive approach produced a suprisingly reasonable value for the lift of approximately 722 $\mathrm{N}$ with approximately a 3.4 \% error compared to the result calculated using the coefficient of lift. 
This is explained by the reasonably uniform spacing of the upper and lower $C_p$ measurements along the airfoil.
Therefore, naively averaging the $C_p$ values with uniform weight does not introduce a large amount of error. 
Perhaps this should not be too much of a suprise as we can write that

$$C_L = \sum_{i=0}^{N-1} (C_p^\mathrm{Lower}[i] - C_p^\mathrm{Upper}[i]) \frac{\Delta x}{c}$$

where $N$ is the number of $C_p$ measurements and $\Delta x / c$ is the distance between $C_p$ measurements normalized by chord length (assumed to be uniform). 
The table below shows that the mean lift has a negligible dependence on the processor used by the Signaloid Cloud Platform. 
The results for variance, skewness, and kurtosis should be ignored for the table below because loading the upper and lower $C_p$ values into distrubtions makes the uncertainty in the result artificially large.
As the uncertain variables $C_p^\mathrm{Lower}$, $C_p^\mathrm{Upper}$, $v_\infty$, and $\rho$ in the program are indepedent, the results for each processor variant are the same with and without autocorellation tracking. 
This same trend is observable for all subsequent experiments.

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift ($\mathrm{N}$) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
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

We needed to remove the oversimplification of treating $C_p$ measurements from different sensors as one uncertain variable.
We therefore edited the code to perform a weighted averaging of the $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ values based upon their relative position on the airfoil, taking into account that there are 29 valid measurement values for $C_p^\mathrm{Upper}$ and only 27 valid measurement values for $C_p^\mathrm{Lower}$. 
The table below shows a slightly improved value for lift of 716.8 $\mathrm{N}$ with 2.70 % error. Now we are not treating the $C_p^\mathrm{Lower} and C_p^\mathrm{Upper}$ values as distributions the variance, skewness, and kurtosis values reflect the uncertainty we introduced in the $v_\infty$ and $\rho$ variables.
We can see small but non-neglible improvements in the accuracy of the variance, skewness, and kurtosis with the increasing precision of the processor used by the Signaloid Cloud Platform.
The plot below the table shows the distribution of the lift calculated using the main branch on a C0-XL processor. 

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift ($\mathrm{N}$) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
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

<img style="text-align:center" width="345" alt="lift" src="https://github.com/user-attachments/assets/c330610b-bc94-4ac0-91dc-b7ecdbfdf5dd">

To address the question of what would happen if we used fewer pressure sensors and therefore obtained fewer measurements for $C_p$, we wrote a program (branch: two-measurements-only) where we drop all but one datapoint for both $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$.
The table of results below show that this has an extreme effect upon the mean lift prediction of the program. 
The predicted value for lift is approximately 1166 $\mathrm{N}$ an error of approximately 67 % when compared to the lift value calculated from the experimentally measured lift coefficient. 
This indicates that making the assumptions we have made and using the data avaliable from [1] the agreement of our calculations with the experimentally measured lift coefficient are limited by the quantity of more $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ measurements along the airfoil. 
The agreement of the lift calculated by our program and the lift calculated from the experimentaly-measured lift coefficient is limited by epistemic uncertainty due to the lack of more $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ measurements.
It is difficult to quantify the effect of aleatoric uncertainty from the pressure sensors without repeated empirical sensor measurements.
As before, we can see non-neglible improvements in the accuracy of the variance, skewness, and kurtosis with the increasing precision of the processor used by the Signaloid Cloud Platform. 
These improvements are neglible compated to the epistemic uncertainty that we have exposed ourselves to by failing to complete more $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ measurements by installing more pitot tubes or alternative pressure measurement sensors. 

| Processor    | Precision            | Memory Size | Correlation Tracking | Mean Lift ($\mathrm{N}$) | Lift Variance ($\mathrm{N^2}$) | Lift Skewness (a.u.) | Lift Kurtosis (a.u.) |
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

## Potential Future Extensions of This Work

1. Model the uncertainty of all the pressure sensors used to measure the 27 $C_p^\mathrm{Lower}$ values and 29 $C_p^\mathrm{Upper}$ values.
2. Run experiments to measure the effect of varying numbers of $C_p^\mathrm{Lower}$ and $C_p^\mathrm{Upper}$ measurements on the calculated lift.
3. Explore the tradeoff of error reduction gained by adding more pressure sensors against the uncertainty of those pressure sensors. We would need to run additional wind tunnel experiments to do this. Or guess the shape and parameters of the distributions quantifying the pressure sensor uncertainty. 
4. Sample from the final lift distribtution using the UxHwDoubleSample function 100,000,000 times and calculate the Wasserstein distance between a 100,000,000 sample reference ground truth Monte Carlo run on my local machine and each of the various processors supplied with the Signaloid Compute Platform listed in the above tables.
5. Find experimental data for an airfoil in a wind tunnel with many repeated raw sensor measurements and re-run the program with empirical distributions loaded into an uncertain variable for each upper and lower $C_p$ value.
6. Re-write a minimal implementation of Xfoil to calculate $C_p$ values using the C programming language with uncertainty information.

## References

[1] https://www.nrel.gov/wind/nwtc/airfoils-osu-data.html

[2] Hoffmann, M. J., R. Reuss Ramsay, and G. M. Gregorek. Effects of grit roughness and pitch oscillations on the NACA 4415 airfoil. No. NREL/TP-442-7815. National Renewable Energy Lab.(NREL), Golden, CO (United States); The Ohio State Univ., Columbus, OH (United States), 1996.

[3] https://ocw.mit.edu/courses/16-01-unified-engineering-i-ii-iii-iv-fall-2005-spring-2006/122da0ffdc4a15d958685358dde118f3_sp7_notes.pdf
