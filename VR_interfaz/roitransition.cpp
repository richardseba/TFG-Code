#include "roitransition.h"
#include <QDebug>

ROITransition::ROITransition()
{
    m_currentRect = NULL;
    m_isTransitionOn = false;

    m_stepX = 0;
    m_stepY = 0;
}
ROITransition::ROITransition(Rect* linkedROIRect)
{
    m_currentRect = linkedROIRect;
    m_isTransitionOn = false;

    m_stepX = 0;
    m_stepY = 0;
}


void ROITransition::setTarget(Rect targetRect, int numberOfSteps)
{
    m_targetRect = targetRect;

    int incX = abs(m_currentRect->x - m_targetRect.x);
    int incY = abs(m_currentRect->y - m_targetRect.y);
    incX = (int)max(incX/numberOfSteps,1);
    incY = (int)max(incY/numberOfSteps,1);


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


    m_currentRect->height = m_targetRect.height;
    m_currentRect->width = m_targetRect.width;

    if(*m_currentRect != m_targetRect)
    {
        m_isTransitionOn = true;
    } else {
        m_isTransitionOn = false;
    }
}

void ROITransition::step()
{
    if(!isOnTarget())
    {
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



















