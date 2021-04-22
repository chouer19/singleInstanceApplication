#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

/* 定义 mutex */
pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;

int main()
{
        int shm_id = 0;
        int i = 0;
        pid_t pid;
        pthread_mutex_t *m_mutex;

        /* mutex attr 初始化 */
        pthread_mutexattr_init(&mutexattr);
        /* 修改属性 */
        pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
        /* mutex 初始化 */
        pthread_mutex_init(&mutex, &mutexattr);

        /* 申请共享内存 */
        shm_id = shmget((key_t)1004, sizeof(mutex), IPC_CREAT | 0600);
        /* 映射共享内存到进程地址空间 */
        m_mutex = (pthread_mutex_t*)shmat(shm_id, 0, 0);
        memcpy(m_mutex, &mutex, sizeof(mutex));

        pid = fork();

        if(pid == 0){
                pthread_mutex_lock(m_mutex);
                ///for(; i<3; i++){
                ///        pthread_mutex_lock(m_mutex);
                ///        puts("This is the child process!");
                ///}
                sleep(10);
        }else if(pid > 0){
                ///for(; i<3; i++){
                ///    sleep(1);
                ///    pthread_mutex_unlock(m_mutex);
                ///    puts("This is the parent process!");
                ///}
                    puts("This is the parent process!");

        /* 回收子进程资源 */
                wait(NULL); 
        }
        /* 销毁 mutex */
        pthread_mutexattr_destroy(&mutexattr);
        pthread_mutex_destroy(&mutex);

        return 0;
}
