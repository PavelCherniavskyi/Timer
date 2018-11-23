#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <tuple>
#include <vector>
#include <memory>

/*! \brief class timer */
template <class Instance, typename... Args>
class Timer
{
public:
    /*! \brief constructor */
    Timer(size_t msecInterval, Instance *inst, void (Instance::*func)(Args...), Args... args)
        : m_StartTime()
        , m_EndTime()
        , m_isRunning(false)
        , m_threadLocal()
        , m_instance(inst)
        , m_function(func)
        , m_callFunction()
        , m_secInterval(msecInterval)
    {
        std::cout << "Timer constructor" << std::endl;
        m_callFunction = [=]() {
            while (m_isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_secInterval));
                if(m_isRunning){
                    (m_instance->*m_function)(args...);
                }
            }
        };
    }

    /*! \brief destructor */
    ~Timer()
    {
        m_isRunning = false;
    }

    /*! \brief do single shot for some function */
    static void singleShot(size_t msecDelay, std::shared_ptr<Instance> inst, void (Instance::*func)(Args...), Args... args)
    {
        m_threadSingleShot.reset(new std::thread([=](){
            std::this_thread::sleep_for(std::chrono::milliseconds(msecDelay));
            ((*inst).*func)(args...);
        }));
        m_threadSingleShot->detach();
    }

    /*! \brief start timer */
    void start(int msec = 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
        m_StartTime = std::chrono::system_clock::now();
        m_isRunning = true;
        m_threadLocal.reset(new std::thread(m_callFunction));
        m_threadLocal->detach();
    }

    /*! \brief stop timer */
    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_isRunning = false;
    }

    /*! \brief get elapsed milliseconds */
    int elapsedMillSec()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if(m_isRunning) {
            endTime = std::chrono::system_clock::now();
        } else {
            endTime = m_EndTime;
        }
        int retval = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count());
        return retval;
    }

private:
    static std::unique_ptr<std::thread>                 m_threadSingleShot;

    std::chrono::time_point<std::chrono::system_clock>  m_StartTime;
    std::chrono::time_point<std::chrono::system_clock>  m_EndTime;
    bool                                                m_isRunning;
    std::unique_ptr<std::thread>                        m_threadLocal;
    Instance                                            *m_instance;
    void                                                (Instance::*m_function)(Args...);
    std::function<void()>                               m_callFunction;
    int                                                 m_secInterval;
};

template <class Instance, typename... Args>
std::unique_ptr<std::thread>    Timer<Instance, Args...>::m_threadSingleShot;
