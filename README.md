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

Let $v$ be the velocity of the air at a specific point in the flow ($\mathrm{m/s}$) and $v_\infty$ be the uniform velocity of the air far upstream from the airfoil ($\mathrm{m/s}$).
Let the coefficient of pressure for each point in the flow be a unitless quantity where

$$C_p = \frac{p-p_\infty}{q_\infty}$$

where $q_\infty = \frac{1}{2} \rho v^2_\infty$. We can re-arrange the equation and substitute $\frac{1}{2} \rho v^2_\infty$ for $q_\infty$ to get

$$p = p_\infty + \frac{1}{2} \rho v^2_\infty C_p$$.

Then $\Delta p = p_\mathrm{Lower} - p_\mathrm{Upper} = \frac{1}{2} \rho v^2_\infty (C_p^\mathrm{Lower}$ - $C_p^\mathrm{Upper})$ as $p_\infty$ is present in the expression for both $p_\mathrm{Lower}$ and $p_\mathrm{Upper}$ and therefore cancels out. Once we have calculated $\Delta p$ the lift is 

$$L = \Delta p A.$$

## Results and Checking Lift Calculation Result Using Experimentally Measured Coefficient of Lift

Run 47 in file N4415c100.txt in the dataset [1] contains an experimentally measured value of $C_L = 1.354$ for the coefficient of lift that we can use to check the result of our program. We can calculate the lift from the coefficient of lift and known parameters 

$$L = C_L q_\infty A = 698 N$$

| Processor    | $C_p$ as distribution | $C_p$ Weighted Average | Drop out half of $C_p$ datapoints |
|--------------|-----------------------|------------------------|-----------------------------------| 
|              | 1.99                  | *7*                    |                                   |
| Bananas      | **1.89**              | 5234                   |                                   |

## References

[1] https://www.nrel.gov/wind/nwtc/airfoils-osu-data.html

[2] Hoffmann, M. J., R. Reuss Ramsay, and G. M. Gregorek. Effects of grit roughness and pitch oscillations on the NACA 4415 airfoil. No. NREL/TP-442-7815. National Renewable Energy Lab.(NREL), Golden, CO (United States); The Ohio State Univ., Columbus, OH (United States), 1996.
