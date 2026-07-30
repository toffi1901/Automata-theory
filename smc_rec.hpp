#ifndef  SMC_H
#define SMC_H

#include "classes.hpp"

class ISmc_Recognizer : public virtual IRecognizers{

};

class Smc_Recognizer : public ISmc_Recognizer{
    Smc_Recognizer() = default;
    ~Smc_Recognizer() = default;
};

#endif