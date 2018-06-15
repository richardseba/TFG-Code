#ifndef DEPTHPROCESSING_H
#define DEPTHPROCESSING_H

#include <QThread>
#include <QMutex>
#include <QTimer>

#include "utils.h"
#include "stereocalibration.h"
#include "classificatorsSrc/thresholdclassificator.h"
#include "libelasSrc/elas.h"

class DepthProcessing : public QObject
{
    Q_OBJECT
public:
    DepthProcessing(QTimer* timer, StereoCalibration stereoCalib,
                     float threshold1, float threshold2, int meanBuffSize,
                     int subsampling=1, Elas::setting elasSetting = Elas::CVC);
    ~DepthProcessing();

    Distance getCurrentDistance();
    float getCurrentDistanceValue();

    void setLibelasSetting(Elas::setting setting);
    void setSubsampling(int subsampl);
    void setImages2Process(QImagePair imageInput);

public slots:
    void setProcessingEvent(bool processing);

private slots:
    void frameProcessingEvent();

private:
    DepthProcessing();
    void setCurrentDistance(Distance dist);
    void setCurrentDistanceValue(float dist);

    Elas::setting getLibelasSetting();
    int getSubsampling();
    QImagePair getImages2Process();

    StereoCalibration m_currentCalib;
    QImagePair m_images2Process;
    int m_subsampling;
    Elas::setting m_libelasSettings;
    ThresholdClassificator m_classifier;

    Distance m_currentDistance;
    float m_currentDistanceValue;

    QTimer* m_timerTrigger;

    QMutex m_mutexDistance;
    QMutex m_mutexDistanceValue;
    QMutex m_mutexSubsampling;
    QMutex m_mutexImages2Process;
    QMutex m_mutexElasSettings;
};

#endif // DEPTHPROCESSING_H
