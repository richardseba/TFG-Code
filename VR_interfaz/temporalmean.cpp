#include "temporalmean.h"

TemporalMean::TemporalMean()
{
    m_bufferSize = DEFAULT_BUFFER_SIZE;
    m_oldMeanVector = new floatInitialized[m_bufferSize];
    m_oldestValue = 0;
    m_meanAlreadyCalculated = false;
}

TemporalMean::TemporalMean(int bufferSize)
{
    m_bufferSize = bufferSize;
    m_oldMeanVector = new floatInitialized[m_bufferSize];
    m_oldestValue = 0;
    m_meanAlreadyCalculated = false;
}


TemporalMean::~TemporalMean()
{
    delete[] m_oldMeanVector;
}

void TemporalMean::setNewValue(float value)
{
    m_oldMeanVector[m_oldestValue].isInit = true;
    m_oldMeanVector[m_oldestValue].value = value;
    m_oldestValue = (m_oldestValue+1)%m_bufferSize;
    m_meanAlreadyCalculated = false;
}

float TemporalMean::getCurrentMean()
{
    if(!m_meanAlreadyCalculated) {
        float sum = 0;
        int count = 0;
        for(int i = 0; i<m_bufferSize; i++){
            sum += m_oldMeanVector[0].value;
            //if is false we will not count that position;
            count += m_oldMeanVector[0].isInit;
        }

        m_currentMeanValue = sum/m_bufferSize;
        m_meanAlreadyCalculated = true;
    }
    return m_currentMeanValue;
}


