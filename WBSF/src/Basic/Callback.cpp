//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     Rémi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
// 01-01-2016	Rémi Saint-Amant	Include into Weather-based simulation framework
//******************************************************************************


#include <iostream>

#include "Basic/UtilStd.h"
#include "Basic/OpenMP.h"


#include "Callback.h"

//voir
//https://github.com/p-ranav/indicators
//https://www.codeproject.com/Articles/21219/Collapsing-Multiple-Progress-Bars


using namespace std;

namespace WBSF
{

CCallback CCallback::DEFAULT_CALLBACK;
//CCriticalSection CCallback::CS;
std::mutex CCallback::CS;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCallback::CCallback() :
    m_cancelEvent(false),
    m_pauseEvent(true)
{
    Reset();
}


CCallback::~CCallback()
{
    Reset();
}

void CCallback::Reset()
{
    //SetUserCancelMessage(WBSF::GetString(IDS_BSC_USER_BREAK));
    //SetUserCancel(false);
    //SetPause(false);


    m_messageAccumulator.clear();
    m_messageDlgAccumulator.clear();
    while (m_tasks.size())
        m_tasks.pop();



//    m_phWnd = NULL;
    m_bPumpMessage = false;

}

CCallback::CCallback(const CCallback& callback)
{
    operator=(callback);
}

CCallback& CCallback::operator=(const CCallback& in)
{
    if (&in != this)
    {
        m_messageAccumulator = in.m_messageAccumulator;
        m_messageDlgAccumulator = in.m_messageDlgAccumulator;
        m_tasks = in.m_tasks;
//        m_phWnd = in.m_phWnd;
    }

    return *this;
}

void CCallback::Lock()
{
    assert(false);
    //int n = omp_get_thread_num();
    //m_mutex.lock();
}

void CCallback::Unlock()
{
    assert(false);
    //int n = omp_get_thread_num();
    //m_mutex.unlock();
}

double CCallback::GetNbStep()const
{

    double nbSteps = 0;
//    CS.Enter();

    if (!GetTasks().empty())
        nbSteps = GetTasks().top().m_nbSteps;

    // CS.Leave();

    return nbSteps;
}

size_t CCallback::GetNbTasks()const
{
    size_t size = 0;
    //CS.Enter();

    size = GetTasks().size();

    //CS.Leave();

    return size;
}

size_t CCallback::GetCurrentLevel()
{
    return GetNbTasks();
}


std::string CCallback::GetMessages()
{
    return m_messageAccumulator;
}

double CCallback::GetCurrentStepPos()const
{
    double stepPos = 0;
    // CS.Enter();

    if (!GetTasks().empty())
        stepPos = GetTasks().top().m_stepPos;

    // CS.Leave();

    return stepPos;
    //return !GetTasks().empty() ? GetTasks().top().m_stepPos:0;
}

ERMsg CCallback::SetCurrentStepPos(double stepPos)
{
    ERMsg msg;

    //if (omp_get_thread_num() == 0)
    //{
    //std::unique_lock<std::mutex> lock(m_mutex);
    //m_mutex.lock();

    //CS.Enter();//problem with wind10???

    if (!GetTasks().empty())
    {
        GetTasks().top().m_stepPos = stepPos;
        if (omp_get_thread_num() == 0)
        {
            GDALStyleProgressBar(GetTasks().top().m_stepPos / GetTasks().top().m_nbSteps);
        }
    }

    if (GetUserCancel())
    {
        assert(!m_userCancelMsg.empty());
        msg.ajoute(m_userCancelMsg);
    }

    //CS.Leave();
    //}

    //if (omp_get_thread_num() == 0)
    //{
    /*	if (GetUserCancel())
    	{
    		assert(!m_userCancelMsg.empty());
    		msg.ajoute(m_userCancelMsg);
    	}
    }*/

    return msg;
}
ERMsg CCallback::SetCurrentStepPos(size_t stepPos)
{
    return SetCurrentStepPos((double)stepPos);
}

ERMsg CCallback::StepIt(double stepBy)
{
    ERMsg msg;

    if (stepBy!=0)
    {
        // CS.Enter();
        if (!GetTasks().empty())
        {
            //step it only apply on the first tread for the moment
            if (stepBy == -1)
                stepBy = GetTasks().top().m_stepBy;

            GetTasks().top().m_stepPos += stepBy;

            if (omp_get_thread_num() == 0)
            {
                GDALStyleProgressBar(GetTasks().top().m_stepPos / GetTasks().top().m_nbSteps);
            }
        }
        //CS.Leave();
    }

    



    if (GetUserCancel())
    {
//        CS.Enter();
        assert(!m_userCancelMsg.empty());
        msg.ajoute(m_userCancelMsg);
        // CS.Leave();
    }
    //}

    return msg;
}

double CCallback::GetStepPercent(size_t i)const
{
    double stepp = 0;
    //CS.Enter();

    if (i < m_tasks.size())
    {
        const WBSF::CCallbackTask& t = m_tasks.c.at(i);
        stepp = t.m_nbSteps > 0 ? std::min(100.0, std::max(0.0, t.m_stepPos*100.0 / t.m_nbSteps)) : 0;
    }

    //CS.Leave();

    return stepp;
}

double CCallback::GetCurrentStepPercent()const
{
    double stepPos = GetCurrentStepPos();
    double nbSteps = GetNbStep();
    return nbSteps != 0 ? std::min(100.0, std::max(0.0, stepPos*100.0 / nbSteps)) : 100.0;

    //return !GetTasks().empty() ? (GetTasks().top().m_nbSteps != 0 ? std::min(100.0, std::max(0.0, GetTasks().top().m_stepPos*100.0 / GetTasks().top().m_nbSteps)) : 100.0) : 0.0;
}

void CCallback::AddMessage(const ERMsg& message, int level)
{
    for (unsigned int i = 0; i < message.dimension(); i++)
        AddMessage(message[i], level);
}

void CCallback::AddMessage(const std::string& message, int level)
{
    AddMessage(message.c_str(), level);
}


void CCallback::AddMessage(const char* message, int level)
{
    //if (omp_get_thread_num() == 0)
    {
        CS.lock();
        if (!GetTasks().empty())
        {
            level = int(GetCurrentLevel()) + std::max(0, level);

            string levelTabs;
            for (int i = 0; i < level; i++)
                levelTabs += "    ";

            string t = message;
            t = ReplaceString(t, "\n", "\n" + levelTabs);

            GetTasks().top().m_messages += levelTabs + t + "\n";
            m_messageAccumulator += levelTabs + t + "\n";
            m_messageDlgAccumulator += levelTabs + t + "\n";


        }
        CS.unlock();
    }

    if (omp_get_thread_num() == 0)
    {
//        if (m_phWnd && *m_phWnd && ::IsWindow(*m_phWnd))
         //   SendMessage(*m_phWnd, WM_MY_THREAD_MESSAGE, 0, 0);
    }
}


void CCallback::DeleteMessages(bool bAccumulation)
{
    if (!GetTasks().empty())
        GetTasks().top().m_messages.clear();

    if (bAccumulation)
        m_messageAccumulator.clear();

}


void CCallback::WaitPause()
{
    //wait when pause is activated
    //m_pauseEvent.wait();
}

void CCallback::PushTask(const std::string& description, double nbStep, double stepBy)
{

    {
//        CS.Enter();
        //std::unique_lock<std::mutex> lock(m_mutex);

        if (nbStep == NOT_INIT)
            nbStep = -1.0;

        GetTasks().push(CCallbackTask(description, nbStep, stepBy));

        // CS.Leave();
    }

    if (omp_get_thread_num() == 0)
    {
        cout << description << endl;
//        if (m_phWnd && *m_phWnd && ::IsWindow(*m_phWnd))
            //SendMessage(*m_phWnd, WM_MY_THREAD_MESSAGE, 1, 0);
    }
}

void CCallback::PopTask()
{
    //if (omp_get_thread_num() == 0)
    //{
    //CS.Enter();
    //std::unique_lock<std::mutex> lock(m_mutex);

    if (!m_tasks.empty())
    {
        //assert(omp_get_thread_num() == 0);

        //transfer message to parent
        string messages;
        if (!GetTasks().empty())
            messages = GetTasks().top().m_messages;

        GetTasks().pop();

        if (!GetTasks().empty())
            GetTasks().top().m_messages += messages;
    }

    //CS.Leave();
    //}

    if (omp_get_thread_num() == 0)
    {
        //unlock callback before notify parent
//        if (m_phWnd && *m_phWnd && ::IsWindow(*m_phWnd))
            //SendMessage(*m_phWnd, WM_MY_THREAD_MESSAGE, 2, 0);
    }
}


//void InitProgressBar(PBD* p, unsigned long MaxLen, char Left, char Right, char Fill, char Empty)
//{
//    p->nMaxLen = MaxLen;
//    p->nCurLen = 0;
//
//    p->LeftMargin = Left;
//    p->RightMargin = Right;
//    p->FillChr = Fill;
//    p->EmptyChr = Empty;
//}
//
//void DrawProgressBar(PBD* p)
//{
//    /** Move to the beginning of the line. */
//    printf("\r");
//
//    /** Print the left margin char. */
//    printf("%c", p->LeftMargin);
//
//    /** Make sure that MaxLen >= CurLen */
//    if (p->nMaxLen < p->nCurLen)
//        p->nCurLen = p->nMaxLen;
//
//    /** Print the progress with the Fill char. */
//    for(unsigned long i = 0; i < p->nCurLen; i++)
//        printf("%c", p->FillChr);
//
//    /** Complete whats left with the Fill char. */
//    for(unsigned long i = 0; i < p->nMaxLen - p->nCurLen; i++)
//        printf("%c", p->EmptyChr);
//
//    /** Print the right margin char. */
//    printf("%c", p->RightMargin);
//}

}//namespace WBSF
