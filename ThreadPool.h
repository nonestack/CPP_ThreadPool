#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <pthread.h>                    //线程库
#include <semaphore.h>                  //信号库

#include <deque>

template<typename T>
class ThreadPool{
    public:
        ThreadPool(int threadnum = 8, int taskque = 10000);
        ~ThreadPool();

        int append(T *task);

    private:                          //成员函数
        static void *worker(void *arg);
        void run();
    private:                          //成员变量
        int thread_number;
        int max_taskque;              //最大等待任务数
        pthread_t *threads;           //线程池的大小
        
        std::deque<T *> task_que;        //任务等待的队列
        sem_t que_state;               //信号量，判断是否有空闲线程
        sem_t que_locker;              //线程池的锁
        int isStop;                  //当前线程是否停止
};

template<typename T>
ThreadPool<T>::ThreadPool(int threadnum, int taskque) :
thread_number(threadnum), max_taskque(taskque), isStop(0)
{
    //if(threadnum <= 0 || taskque <= 0){
    //    //异常处理
    //}
    threads = new pthread_t[thread_number];
    if(threads == NULL){
        //异常处理
    }
    for(int i = 0; i < thread_number; ++i){
        std::cout << "Hello" << std::endl;
        int ret = pthread_create(threads + i, NULL, worker, this);
        if(ret != 0){
            //异常处理
        }
        if(pthread_detach(threads[i]) != 0){
            //异常处理
        }
    }

    sem_init(&que_state, 0, thread_number);                          //有多少空闲线程
    sem_init(&que_locker, 0, 1);                                     //互斥锁
    //isStop = 0;                                                      //如果不初始化isStop，创建临时变量的线程池时，isStop是一个随机值
};

template<typename T>
ThreadPool<T>::~ThreadPool(){
    delete[] threads;
    isStop = 1;
}

template<typename T>
int ThreadPool<T>::append(T *task){
    sem_wait(&que_locker);
    if(task_que.size() > max_taskque){                              //如果任务队列已满，返回错误
        sem_post(&que_locker);
        return 0;
    }

    task_que.push_back(task);
    sem_post(&que_locker);                                          //解锁
    sem_post(&que_state);                                           //信号通知，任务队列又有任务

    return 1;
}

template<typename T>
void *ThreadPool<T>::worker(void *arg){
    ThreadPool *pool = (ThreadPool *)arg;
    std::cout << "World" << std::endl;
    pool->run();
    return pool;
}

template<typename T>
void ThreadPool<T>::run(){
    std::cout << "isStop = " << isStop << std::endl;
    while(!isStop){
        std::cout << "ok" << std::endl;
        sem_wait(&que_state);                    //检查队列里是否有任务
        sem_wait(&que_locker);                   //检查线程池现在是否正在使用
        if(task_que.empty()){                    //判断现在是否有任务，没有任务，从头继续等待
            sem_post(&que_locker);
            continue;
        }
        T *task = task_que.front();
        task_que.pop_front();
        sem_post(&que_locker);

        if(task == NULL){
            continue;
        }

        task->run();                             //class T 要包含run()
    }
}

#endif