#include "Circuit.h"

Circuit::Circuit(double resistance, double inductance, double capacitance, double iCurrent, double iVoltage, bool inSeries){
    this->R = resistance;
    this->L = inductance;
    this->C = capacitance;
    this->iCurrent = iCurrent;
    this->iVoltage = iVoltage;
    this->inSeries = inSeries;

    omega0 = 1/sqrt(L*C);
    double initial, other, multiplier;

    if(inSeries){
        alpha = R/(2*L);
        initial = iCurrent;
        other = iVoltage;
        multiplier = L;
    }
    else{
        alpha = 1/(2*R*C);
        initial = iVoltage;
        other = iCurrent;
        multiplier = C;
    }

    calcResponseType();

    switch(response){
        case OVER:
            s1 = -alpha + sqrt(alpha*alpha - omega0*omega0);
            s2 = -alpha - sqrt(alpha*alpha - omega0*omega0);

            solveSystem();
            break;

        case UNDER:
            omegaD = sqrt((omega0*omega0)-(alpha*alpha));

            a1 = initial;
            a2 = ((other/multiplier) + alpha*a1)/omegaD;
            break;

        case CRITICALLY:
            s1 = -alpha + sqrt(alpha*alpha - omega0*omega0);
            s2 = -alpha - sqrt(alpha*alpha - omega0*omega0);

            a2 = initial;
            a1 = (other/multiplier) + alpha*a2;
            break;
    }
}

void Circuit::calcResponseType(){
    double result = alpha*alpha - omega0*omega0;

    if(result > 0)
            response = OVER;
        else if(result < 0)
            response = UNDER;
        else
            response = CRITICALLY;
}

void Circuit::solveSystem(){
    int i, j, k;

    double m[2][3], a[2];

    m[0][0] = 1;
    m[0][1] = 1;
    if(inSeries)
        m[0][2] = iCurrent;
    else
        m[0][2] = iVoltage;

    m[1][0] = s1;
    m[1][1] = s2;
    if(inSeries)
        m[1][2] = iVoltage/L;
    else
        m[1][2] = iCurrent/C;


    for(i = 0; i < 2; i++)                            //Pivotisation
        for(k = i+1; k < 2; k++)
            if(abs(m[i][i]) < abs(m[k][i]))
                for(j = 0; j <= 2; j++){
                    double aux = m[i][j];
                    m[i][j] = m[k][j];
                    m[k][j] = aux;
                }


    for(i = 0; i < 1; i++)                            //loop to perform the gauss elimination
        for(k = i+1; k < 2; k++){
                double t = m[k][i]/m[i][i];
                for(j = 0; j <= 2; j++)
                    m[k][j] = m[k][j] - t*m[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
            }

    for(i = 1; i>=0 ;i--){                            //back-substitution
        a[i] = m[i][2];                               //make the variable to be calculated equal to the rhs of the last equation
        for(j = i+1; j < 2; j++)
            if(j != i)                                //then subtract all the lhs values except the coefficient of the variable whose value is being calculated
                a[i] = a[i] - m[i][j] * a[j];
        a[i] = a[i] / m[i][i];                        //now finally divide the rhs by the coefficient of the variable to be calculated
    }

    a1 = a[0];
    a2 = a[1];
}

void Circuit::printData(){
    std::string responseString;

    switch(response){
        case OVER:
            printf("Response type: OVERDAMPED\n\n");
            break;

        case UNDER:
            printf("Response type: UNDERDAMPED\n\n");
            break;

        case CRITICALLY:
            printf("Response type: CRITICALLY DAMPED\n\n");
            break;
    }

    printf("Alpha = %.2lf rad/s\n", alpha);
    printf("Omega0 = %.2lf rad/s\n\n", omega0);

    if(response == (OVER || CRITICALLY)){
        printf("s1 = %.2lf rad/s\n", s1);
        printf("s1 = %.2lf rad/s\n\n", s1);
    }
    else
        if(response == UNDER)
            printf("omegaD = %.2lf rad/s\n\n", omegaD);

    printf("Formula: ");

        switch(response){
        case OVER:
            printf("%s %.2lf*e^(%.2lf*t) + %.2lf*e^(%.2lf*t) (%c)\n", inSeries?"i(t) =":"v(t) =", a1, s1, a2, s2, inSeries?'A':'V');
            break;

        case UNDER:
            printf("%s e^(%.2lf*t) * (%.2lf*cos(%.2lf*t) + %.2lf*sen(%.2lf*t) ) (%c)\n", inSeries?"i(t) =":"v(t) =", -alpha, a1, omegaD, a2, omegaD, inSeries?'A':'V');
            break;

        case CRITICALLY:
            printf("%s e^(%.2lf*t) * (%.2lf*t + %.2lf) (%c)\n", inSeries?"i(t) =":"v(t) =", -alpha, a1, a2, inSeries?'A':'V');
            break;
    }
}
