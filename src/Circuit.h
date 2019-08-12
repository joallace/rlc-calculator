#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <cstdio>

#include <string>
#include <cmath>

typedef enum {OVER, CRITICALLY, UNDER} tResponse; //DAMPED

class Circuit{
    public:
        Circuit(double resistance, double inductance, double capacitance, double iCurrent, double iVoltage, bool inSeries);

        bool inSeries;

        double iCurrent,
               iVoltage;

        double alpha,
               omega0,
               omegaD;

        double a1,
               a2;

        double s1,
               s2;

        double R,
               L,
               C;

        tResponse response;

        void calcResponseType();
        void solveSystem();

        void printData();
};

#endif // CIRCUIT_H
