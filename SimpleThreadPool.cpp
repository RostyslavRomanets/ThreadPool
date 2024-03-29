//
// Created by Rostik on 3/27/2024.
//

#include "SimpleThreadPool.h"

SimpleThreadPool::SimpleThreadPool(size_t threadCount)
        : m_threadCount(threadCount) {
    m_threads.reserve(threadCount);
    try
    {
        for (int i = 0; i < m_threadCount; i++)
        {
            m_threads.emplace_back(&SimpleThreadPool::worker, this);
        }
    }
    catch (...)
    {
        {
            std::lock_guard<std::mutex> lock(m_mut);
            m_stop = true;
        }
        m_notification.notify_all();
        for (auto &thread : m_threads)
        {
            if (thread.joinable())
                thread.join();
        }

        throw;
    }
}


SimpleThreadPool::~SimpleThreadPool() {
    Destroy();
}

void SimpleThreadPool::worker() {
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_mut);

            while (m_tasks.empty()) {
                if (m_stop)
                    return;

                m_notification.wait(lock);
            }

            task = std::move(m_tasks.front());
            m_tasks.pop();
        }
        task();
    }
}

void SimpleThreadPool::Destroy() {
    {
        std::lock_guard<std::mutex> lock(m_mut);
        m_stop = true;
    }
    m_notification.notify_all();
    for (auto &thread : m_threads)
    {
        if (thread.joinable())
            thread.join();
    }
}

