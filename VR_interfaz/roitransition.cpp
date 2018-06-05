#include "roitransition.h"
#include <QDebug>

ROITransition::ROITransition()
{
    m_currentRect = NULL;
    m_isTransitionOn = false;
    m_isOnCorrectZoom = false;

    m_stepX = 0;
    m_stepY = 0;
}
ROITransition::ROITransition(Rect* linkedROIRect)
{
    m_currentRect = linkedROIRect;
    m_isTransitionOn = false;
    m_isOnCorrectZoom = true;

    m_stepX = 0;
    m_stepY = 0;
}


void ROITransition::setTarget(Rect targetRect, int numberOfSteps)
{
    m_targetRect = targetRect;

    int incX = abs(m_currentRect->x - m_targetRect.x);
    int incY = abs(m_currentRect->y - m_targetRect.y);
    int incW = abs(m_currentRect->width - m_targetRect.width);
    int incH = abs(m_currentRect->height - m_targetRect.height);

    incX = (int)max(incX/numberOfSteps,1);
    incY = (int)max(incY/numberOfSteps,1);
//    incW = (int)max(incW/numberOfSteps,1);
//    incH = (int)max(incH/numberOfSteps,1);
    incW = 8;
    incH = 9;
    qDebug() << incX << incY;

    if(m_currentRect->x > m_targetRect.x)
    {
        m_stepX = -incX;
        m_XOrientation = goingRight;
    }
    else
    {
        m_stepX = incX;
        m_XOrientation = goingLeft;
    }
    if(m_currentRect->y > m_targetRect.y)
    {
        m_stepY = -incY;
        m_YOrientation = goingDown;
    }
    else
    {
        m_stepY = incY;
        m_YOrientation = goingUp;
    }

    if(m_currentRect->width > m_targetRect.width)
    {
        m_stepW = -incW;
        m_WOrientation = decreasing;
    }
    else
    {
        m_stepW = incW;
        m_WOrientation = growing;
    }

    if(m_currentRect->height > m_targetRect.height)
    {
        m_stepH = -incH;
        m_HOrientation = decreasing;
    }
    else
    {
        m_stepH = incH;
        m_HOrientation = growing;
    }

    m_isOnCorrectZoom = false;
    if(m_currentRect->width == m_targetRect.width && m_currentRect->height == m_targetRect.height)
        m_isOnCorrectZoom = true;

    if(*m_currentRect != m_targetRect)
    {
        m_isTransitionOn = true;
    } else {
        m_isTransitionOn = false;
    }
    qDebug() << "correctZoom" << m_isOnCorrectZoom;
}

void ROITransition::step()
{
    if(!isOnTarget())
    {
        if(!m_isOnCorrectZoom)
        {
            if(m_currentRect->width != m_targetRect.width)
                if(m_WOrientation == growing)
                {
                    if(m_currentRect->width+m_stepW < m_targetRect.width)
                        m_currentRect->width += m_stepW;
                    else //if the step+x is overpasses target x we put target x as position x
                        m_currentRect->width = m_targetRect.width;
                } else { //the target is at the decreasing
                    if(m_currentRect->width+m_stepW > m_targetRect.width)
                        m_currentRect->width += m_stepW;
                    else
                        m_currentRect->width = m_targetRect.width;
                }
            if(m_currentRect->height != m_targetRect.height)
                if(m_HOrientation == growing)
                {
                    if(m_currentRect->height+m_stepH < m_targetRect.height)
                        m_currentRect->height += m_stepH;
                    else //if the step+x is overpasses target x we put target x as position x
                        m_currentRect->height = m_targetRect.height;
                } else { //the target is at the decreasing
                    if(m_currentRect->height+m_stepH > m_targetRect.height)
                        m_currentRect->height += m_stepH;
                    else
                        m_currentRect->height = m_targetRect.height;
                }
        }else {
            qDebug() << "step in movement";
            if(m_currentRect->x != m_targetRect.x)
                if(m_XOrientation == goingLeft)
                {
                    if(m_currentRect->x+m_stepX < m_targetRect.x)
                        m_currentRect->x += m_stepX;
                    else //if the step+x is overpasses target x we put target x as position x
                        m_currentRect->x = m_targetRect.x;
                } else { //the target is at the right
                    if(m_currentRect->x+m_stepX > m_targetRect.x)
                        m_currentRect->x += m_stepX;
                    else
                        m_currentRect->x = m_targetRect.x;
                }

            if(m_currentRect->y != m_targetRect.y)
                if(m_YOrientation == goingUp)
                {
                    if(m_currentRect->y+m_stepY < m_targetRect.y)
                        m_currentRect->y += m_stepY;
                    else //if the step+x is overpasses target x we put target x as position x
                        m_currentRect->y = m_targetRect.y;
                } else { //the target is down
                    if(m_currentRect->y+m_stepY > m_targetRect.y)
                        m_currentRect->y += m_stepY;
                    else
                        m_currentRect->y = m_targetRect.y;
                }
        }
        qDebug() << m_currentRect->width << m_targetRect.width << m_currentRect->height << m_targetRect.height;
        if(m_currentRect->width == m_targetRect.width && m_currentRect->height == m_targetRect.height)
            m_isOnCorrectZoom = true;
        if(*m_currentRect == m_targetRect)
        {
            m_isTransitionOn = false;
        }
    }
}

bool ROITransition::isOnTarget()
{
    if(*m_currentRect == m_targetRect)
    {
        m_isTransitionOn = false;
    }
    return !m_isTransitionOn;
}

void ROITransition::cancelTransition()
{
    m_isTransitionOn = false;
}



















