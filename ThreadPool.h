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

    private:                          //成员函数
        static void *worker(void *arg);
        void run();
    private:                          //成员变量
        int thread_number;
        int max_taskque;              //最大等待任务数
        pthread_t *threads;           //线程池的大小
        
        std::deque<T> taskque;        //任务等待的队列
        sem_t questate;               //信号量，判断是否有任务在等待执行
        bool isStop;                  //当前线程是否停止
};

template<typename T>
ThreadPool<T>::ThreadPool(int threadnum, int taskque) :
thread_number(threadnum), max_taskque(taskque)
{
    //if(threadnum <= 0 || taskque <= 0){
    //    //异常处理
    //}
    threads = new pthread_t[thread_number];
    if(threads == NULL){
        //异常处理
    }
    for(int i = 0; i < thread_number; ++i){
        int ret = pthread_create(threads + i, NULL, worker, this);
        if(ret != 0){
            //异常处理
        }
        if(pthread_detach(threads[i]) != 0){
            //异常处理
        }
    }
}

template<typename T>
ThreadPool<T>::~ThreadPool(){
    delete[] threads;
    isStop = true;
}

template<typename T>
void *Thread<T>::worker(void *arg){

}

#endif