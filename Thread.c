#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

struct exe_data {
    int start;
    int end;
};

#define MAX 100000 // we have this many elements in the array
#define NUM_THREADS 4 // number of threads to use
int data[MAX];
int real_sum = 0; 
int thread_sums[NUM_THREADS] = {0}; // array to hold the sum from each thread

void populate()
{
    for (int i = 0; i < MAX; i++) {
        int val = rand() % 100; // keep the number below some value, if not sum might be too big
        data[i] = val;
        real_sum += val;
    }
}

void *thread(void *arg)
{
    struct exe_data *info = (struct exe_data *)arg;
    int start = info->start;
    int end = info->end;
    int thread_id = (start / (MAX / NUM_THREADS)); // Determine the thread ID based on the start index

    for (int i = start; i < end; i++)
        thread_sums[thread_id] += data[i];

    return NULL;
}

int main()
{
    populate();

    pthread_t tids[NUM_THREADS];
    struct exe_data thd_info[NUM_THREADS];
    int chunk_size = MAX / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thd_info[i].start = i * chunk_size;
        thd_info[i].end = (i + 1) * chunk_size;
        pthread_create(&tids[i], NULL, thread, &thd_info[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    // Sum the results from each thread
    int total_thread_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_thread_sum += thread_sums[i];
    }

    printf("Real sum %d, thread says %d\n", real_sum, total_thread_sum);
    assert(real_sum == total_thread_sum);

    return 0;
}