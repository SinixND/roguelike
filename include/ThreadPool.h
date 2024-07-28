// ThreadPool.h
// https://stackoverflow.com/a/32593825/21874203

#ifndef IG20240526193442
#define IG20240526193442

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// Usage: thread_pool->queueJob([/* =, & */] { /* ... */ });
class ThreadPool
{
public:
    // Add a new job to the pool; use a lock so that there isn't a data race.
    void queueJob(const std::function<void()>& job);

    // ThreadPool::Stop will not terminate any currently running jobs, it just waits for them to finish via active_thread.join().
    void stop();

    // Returns if there are queued jobs left
    // The isJobQueued() function can be used in a while loop, such that the main thread can wait the threadpool to complete all the tasks before calling the threadpool destructor.
    bool isJobQueued();

    // Waits for all jobs to finish without stopping threadpool
    void joinJobs();

    ThreadPool();
    ~ThreadPool();

private:
    // Each thread should be running its own infinite loop, constantly waiting for new tasks to grab and run.
    void start();

    // The infinite loop function. This is a while (true) loop waiting for the task queue to open up.
    void threadLoop();

    // Tells threads to stop looking for jobs
    bool should_terminate = false;

    // Prevents data races to the job queue
    std::mutex queue_mutex;

    // Allows threads to wait on new jobs or termination
    std::condition_variable mutex_condition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

    // Allows to check for active jobs; Needed as threads are always active (waiting for jobs)
    std::mutex active_job_mutex;
    int active_jobs{0};
};

// ThreadPool.cpp
// The anonymous code blocks are used so that when they are exited, the std::unique_lock variables created within them go out of scope, unlocking the mutex.

inline ThreadPool::ThreadPool()
{
    start();
}

inline void ThreadPool::start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports

    threads.reserve(num_threads);

    for (uint32_t i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(std::thread(&ThreadPool::threadLoop, this));
    }
}

inline void ThreadPool::threadLoop()
{
    while (true)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            mutex_condition.wait(
                lock,
                [this]
                {
                    return !jobs.empty() || should_terminate;
                });

            if (should_terminate)
            {
                return;
            }

            job = jobs.front();

            jobs.pop();
        }

        {
            std::unique_lock<std::mutex> lock(active_job_mutex);
            ++active_jobs;
        }

        job();

        {
            std::unique_lock<std::mutex> lock(active_job_mutex);
            --active_jobs;
        }
    }
}

inline void ThreadPool::queueJob(const std::function<void()>& job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }

    mutex_condition.notify_one();
}

inline bool ThreadPool::isJobQueued()
{
    bool isPoolBusy;

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        isPoolBusy = !jobs.empty();
    }

    return isPoolBusy;
}

inline void ThreadPool::joinJobs()
{
    while (active_jobs)
        ;
}

inline void ThreadPool::stop()
{
    // Only need to check for empty queue, because running jobs will be finished before threads are joined
    while (isJobQueued())
        ;

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }

    mutex_condition.notify_all();

    for (std::thread& active_thread : threads)
    {
        active_thread.join();
    }

    threads.clear();
}

inline ThreadPool::~ThreadPool()
{
    stop();
}

#endif