#include "ex3.h"

bounded_array::bounded_array(size_t size)
{
    size_of_array = size;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_array, 0, size);
    sem_init(&full_array, 0, size);
    message_array = new message*[size];
    //thead will always be 0
    head_index = 0;
    tail_index = 0;
}

bounded_array::~bounded_array()
{
    sem_close(&full_array);
    sem_close(&empty_array);
    pthread_mutex_destroy(&mutex);
    // Just in case, we'll try to delete the messages remaining in the array.
    // Generally we expect the array to be empty at this point
    while (head_index != tail_index) {
        delete message_array[head_index];
        head_index++;
        if (head_index >= size_of_array)
            head_index = 0;
    }
    delete [] message_array;
}

void bounded_array::enqueue(message *msg)
{
    sem_wait(&full_array);
    pthread_mutex_lock(&mutex);
    //push the msg and update tail
    message_array[tail_index] = msg;
    tail_index++;
    if (tail_index >= size_of_array)
        tail_index = 0;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_array);
}

message* bounded_array::dequeue(void)
{
    message* ret_message = NULL;
    sem_wait(&empty_array);
    pthread_mutex_lock(&mutex);
    //pop the msg and update tail
    ret_message = message_array[head_index];
    head_index++;
    if (head_index >= size_of_array)
        head_index = 0;
    pthread_mutex_unlock(&mutex);
    sem_post(&full_array);
    return ret_message;
}

//==============================================================================

unbounded_array::unbounded_array()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_array, 0, 0);
}

unbounded_array::~unbounded_array()
{
    sem_close(&empty_array);
    pthread_mutex_destroy(&mutex);
    // Just in case, we'll try to delete the messages remaining in the array.
    // Generally we expect the array to be empty at this point
    while (message_array.size() > 0) {
        message *cur_message = message_array.back();
        message_array.pop_back();
        delete cur_message;
    }
    message_array.clear();
}

void unbounded_array::enqueue(message *msg)
{
    pthread_mutex_lock(&mutex);
    message_array.push_front(msg);
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_array);
}

message* unbounded_array::dequeue(void)
{
    message* ret_message;
    sem_wait(&empty_array);
    pthread_mutex_lock(&mutex);
    ret_message = message_array.back();
    message_array.pop_back();
    pthread_mutex_unlock(&mutex);
    return ret_message;
}
