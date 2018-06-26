#ifndef TEMPORALMEAN_H
#define TEMPORALMEAN_H

const int DEFAULT_BUFFER_SIZE = 33;

typedef struct {
    float value = 0;
    bool isInit = false;
} floatInitialized;

/* Class TemporalMean
 * -------------------------------
 * This class is build to store an array of values and calculate the mean of them.
 * When a new value is set in the array the oldest value gets replaced by the new value
*/
class TemporalMean
{
public:
    TemporalMean();
    TemporalMean(int bufferSize);
    ~TemporalMean();

    void setNewValue(float value);
    float getCurrentMean();
        int m_oldestValue;
private:
    floatInitialized* m_oldMeanVector;

    bool m_meanAlreadyCalculated;
    float m_currentMeanValue;

    int m_bufferSize;
};

#endif // TEMPORALMEAN_H
