#ifndef PRESETCLASSIFICATOR_H
#define PRESETCLASSIFICATOR_H

enum Distance {CLOSE,MEDIUM,FAR};

class PresetClassificator
{
public:
    PresetClassificator();
    Distance calcClasificationProximity(Mat imgL, Mat imgR) = 0;
    float calcClasificationFloat(Mat imgL, Mat imgR) = 0;
    void saveClasificator(char* namefile);
};

#endif // PRESETCLASSIFICATOR_H
