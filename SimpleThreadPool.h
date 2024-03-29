//
// Created by Rostik on 3/27/2024.
//

#ifndef THREADPOOL_SIMPLETHREADPOOL_H
#define THREADPOOL_SIMPLETHREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <future>

class SimpleThreadPool {
public:
    explicit SimpleThreadPool(size_t threadCount = std::thread::hardware_concurrency());

    ~SimpleThreadPool();

    SimpleThreadPool(const SimpleThreadPool & ) = delete;
    SimpleThreadPool & operator=(const SimpleThreadPool & ) = delete;

    template <class Fnc_T>
    auto Post(Fnc_T task) -> std::future<decltype(task())>;

    void Destroy();

private:
    void worker();

private:
    size_t m_threadCount;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::condition_variable m_notification;
    std::mutex m_mut;
    bool m_stop = false;
};

template <class Fnc_T>
auto SimpleThreadPool::Post(Fnc_T task) -> std::future<decltype(task())>
{
    using return_type = decltype(task());

    auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Fnc_T>(task)));
    std::future<return_type> res = task_ptr->get_future();

    {
        std::lock_guard<std::mutex> lock(m_mut);
        m_tasks.emplace([task_ptr]() { (*task_ptr)(); });
    }

    m_notification.notify_one();
    return res;
}

#endif //THREADPOOL_SIMPLETHREADPOOL_H
