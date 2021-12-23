#include "KTimer.h"

float g_fTimer = 0.0f;
float g_fSecPerFrame = 0.0f;

bool KTimer::Init()
{
    return true;
}

bool KTimer::Frame()
{
    return true;
}

bool KTimer::Render()
{
    return true;
}

bool KTimer::Release()
{
    return true;
}

KTimer::KTimer()
{
    m_fFramePerSec = 0;
    m_fGameTimer = 0.0f;
    QueryPerformanceFrequency(&m_Frequency);
}

KTimer::~KTimer()
{
}
