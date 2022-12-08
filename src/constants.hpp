#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

/*
    FILE: constants.hpp
    DESCRIPTION: File containing macros of constants used to parametrize genetic algorithm and plant transfer function.
*/

/* Plant transfer function coefficients 
        A0 + A1*s
  ----------------------
    B0 + B1*s + B2*s^2
*/

#define A0 60.24
#define A1 0.02
#define B0 14.2
#define B1 4.11
#define B2 9.17

#define INIT_SAMPLE_N 2         // number of initial samples for simulation

#define dt 0.001                 // timestep for simulation [s]
#define Ts 40.0                 // simulation time [s]

#define UNSTABLE_COST 1000000.0

#endif