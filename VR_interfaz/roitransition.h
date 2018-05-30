#ifndef ROITRANSITION_H
#define ROITRANSITION_H


//Load OpenCV libraries
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

enum Orientation { goingUp=0, goingDown=1, goingLeft=2, goingRight=3, growing=4, decreasing=5 };

class ROITransition
{
public:
    ROITransition();
    ROITransition(Rect* linkedROIRect);
    void setTarget(Rect targetRect, int numberOfSteps=100);
    void step();
    bool isOnTarget();
    void cancelTransition();
private:
    Rect* m_currentRect;
    Rect m_targetRect;

    int m_stepX;
    Orientation m_XOrientation;

    int m_stepY;
    Orientation m_YOrientation;

    int m_stepW;
    Orientation m_WOrientation;

    int m_stepH;
    Orientation m_HOrientation;



    bool m_isTransitionOn;
    bool m_isOnCorrectZoomm;
};

#endif // ROITRANSITION_H
