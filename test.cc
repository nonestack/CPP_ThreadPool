#include <unistd.h>
#include <stdlib.h>

#include <iostream>

#include "ThreadPool.h"


class Test{
    public:
        Test(int num): id(num){
            std::cout << "Test() : id = " << this->id << std::endl;
        }
        ~Test(){
            std::cout << "~Test() : id = " << this->id << std::endl;
        }

        void run(){
            for(int i = 0; i < 5; ++i){
                std::cout << "task" << id << ": 正在执行" << std::endl;
                sleep(5);
            }
        }

    private:
        int id;                     //用于区分不同的Test对象
};

void computer_number(int number){
    //ThreadPool<Test> *pool = new ThreadPool<Test>();
    ThreadPool<Test> pool{8, 1000};
    for(int i = 0; i < number; ++i){
        Test *tmp = new Test(i);
        //Test tmp{i};
        //pool->append(tmp);
        pool.append(tmp);
        sleep(2);
    }
    while(1){}
    //delete pool;

}


int main(int argc, char *argv[]){
    if(argc != 2){
        std::cout << "usage: program task_number" << std::endl;
        exit(1);
    }
    if(argv[1] <= 0){
        std::cout << "task number illegel" << std::endl;
        exit(1);
    }
    int num = atoi(argv[1]);
    computer_number(num);
    return 0;
}
