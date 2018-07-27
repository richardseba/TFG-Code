#ifndef THRESHOLDCLASSIFICATOR_H
#define THRESHOLDCLASSIFICATOR_H

#include "./classificatorsSrc/presetclassificator.h"
#include "temporalmean.h"

/* Class ThresholdClassificator
 * -------------------------------
 * This class is implements a threshold clasifficator method that returns the distance,
 * as values or class
*/
class ThresholdClassificator : public PresetClassificator
{
public:
    ThresholdClassificator();
    ThresholdClassificator(int numMeanStored, float threshold1,float threshold2);
    Distance calcClasificationProximity(Mat imgL, Mat imgR);
    float calcClasificationFloat(Mat imgL, Mat imgR);
    void saveClasificator(char* namefile){}
private:
    float m_th1;
    float m_th2;
    TemporalMean m_oldMean;
};

#endif // THRESHOLDCLASSIFICATOR_H
