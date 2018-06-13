#ifndef THRESHOLDCLASSIFICATOR_H
#define THRESHOLDCLASSIFICATOR_H

#include "./classificatorsSrc/presetclassificator.h"
#include "temporalmean.h"

class ThresholdClassificator : public PresetClassificator
{
public:
    ThresholdClassificator();
    ThresholdClassificator(int numMeanStored,);
    Distance calcClasificationProximity(Mat imgL, Mat imgR);
    float calcClasificationFloat(Mat imgL, Mat imgR);
//    void saveClasificator(char* namefile);
private:
    float m_th1;
    float m_th2;
    TemporalMean m_oldMean;

};

#endif // THRESHOLDCLASSIFICATOR_H
