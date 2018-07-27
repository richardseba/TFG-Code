#ifndef PRESETCLASSIFICATOR_H
#define PRESETCLASSIFICATOR_H

#include "utils.h"

enum Distance {CLOSE,MEDIUM,FAR};

/* Class ABSTRACT PresetClassificator
 * -------------------------------
 * This abstract class is designed to implement a clasifficator method that returns the distance,
 * as values or class
*/
class PresetClassificator
{
public:
    PresetClassificator(){}
    virtual Distance calcClasificationProximity(Mat imgL, Mat imgR) = 0;
    virtual float calcClasificationFloat(Mat imgL, Mat imgR) = 0;
    virtual void saveClasificator(char* namefile) = 0;
};

#endif // PRESETCLASSIFICATOR_H
