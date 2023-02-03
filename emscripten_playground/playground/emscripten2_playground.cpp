#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __EMSCRIPTEN_PTHREADS__
#error dsfsdg
#endif

void* thread_callback(void *arg)
{
    puts("(puts) Inside the thread 1");
    sleep(1);
    printf("Inside the thread 1: %d\n", *(int *)arg);
    sleep(2);
    printf("Inside the thread 2: %d\n", *(int *)arg);
    return nullptr;
}

int main()
{
    setbuf(stdout, NULL);

    puts("Before the thread");

    pthread_t thread_id;
    int arg = 42;
    pthread_create(&thread_id, NULL, thread_callback, &arg);

    pthread_join(thread_id, NULL);
    sleep(1);

    puts("After the thread");

    return 0;
}
