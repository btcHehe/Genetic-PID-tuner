#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

/* Plant transfer function coefficients 
        A0 + A1*s
  ----------------------
    B0 + B1*s + B2*s^2
*/
#define A0 1.24
#define A1 0.0          // 2.61 is interesting
#define B0 1.2
#define B1 2.11
#define B2 4.17

#define INIT_SAMPLE_N 3        // number of initial samples for simulation

#endif