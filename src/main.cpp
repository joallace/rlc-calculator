#include "Circuit.cpp"

#define ESC 27

void printMenu(){
    system("clear");
    printf("------======\\ RLC Circuit Calculator /======------\n"
           "\t1. Series circuit\n"
           "\t2. Parallel circuit\n"
           "\tEsc. Exit\n\n"

           "Select an option: ");
}

double readDbl(){
    std::string aux;
    double value;

    std::cin >> aux;
    value = stod(aux);

    switch( aux[aux.length()-1] ){
        case 'M':
            value *= 1000000;
            break;

        case 'k':
            value *= 1000;
            break;

        case 'm':
            value /= 1000;
            break;

        case 'u':
            value /= 1000000;
            break;

        case 'n':
            value /= 1000000000;
            break;

        case 'p':
            value /= 1000000000000;
            break;

        default:
            break;
    }

    printf("VALUE: %.12lf\n", value);
    return value;
}

void press2Continue(){
    printf("\n\nPress ENTER key to Continue\n");
    std::cin.ignore();
    getchar();
}

void getValues(double *resistance, double *inductance, double *capacitance, double *iCurrent, double *iVoltage, bool inSeries){
    system("clear");
    printf("------======\\ %s RLC Circuit Calculator /======------\n", inSeries?"Series":"Parallel");

    printf("Resistance: ");
    *resistance = readDbl();

    printf("Inductance: ");
    *inductance = readDbl();

    printf("Capacitance: ");
    *capacitance = readDbl();

    printf("Initial current: ");
    *iCurrent = readDbl();

    printf("Initial voltage: ");
    *iVoltage = readDbl();
}

int main(){
    Circuit *c;

    double resistance,
           inductance,
           capacitance,
           iCurrent,
           iVoltage;

    char nav;
    bool invalidKey = false;

    while(true){
        printMenu();

        if(invalidKey){
            printf("Invalid option!\n");
            invalidKey = false;
        }

        std::cin >> nav;

        switch(nav){
            case '1':
                getValues(&resistance, &inductance, &capacitance, &iCurrent, &iVoltage, true);
                c = new Circuit(resistance, inductance, capacitance, iCurrent, iVoltage, true);

                system("clear");
                c->printData();
                press2Continue();

                break;

            case '2':
                getValues(&resistance, &inductance, &capacitance, &iCurrent, &iVoltage, false);
                c = new Circuit(resistance, inductance, capacitance, iCurrent, iVoltage, false);

                system("clear");
                c->printData();
                press2Continue();

                break;

            case ESC:
                return 0;
                break;

            default:
                invalidKey = true;
                break;
        }
    }
}
