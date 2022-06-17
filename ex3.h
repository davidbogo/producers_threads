#include <semaphore.h>
#include <list>
#include <iostream>
#include <string>
#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_MESSAGE_LEN		255
using namespace std;

enum news_type
{
    NT_SPORT,
    NT_WEATHER,
    NT_NEWS,
    NT_TERMINATE
};

struct producer_info
{
    int producer_no;
    size_t num_of_messages;
    size_t queue_size;
};

struct message
{
    int producer_no;
    news_type type;
    int message_no;
};

class thread_run
{
public:
    thread_run()
    {
    }
    ~thread_run()
    {
    }
    virtual void run()=0;
};

class bounded_array
{
    size_t              size_of_array;
    int                 head_index;
    int                 tail_index;
    pthread_mutex_t     mutex;
    sem_t               empty_array;
    sem_t               full_array;
    message**           message_array;

public:
    bounded_array(size_t size);
    ~bounded_array();
    message* get_message_array(); 
    void enqueue(message *msg);
    message* dequeue(void);
};

class unbounded_array
{
    pthread_mutex_t	mutex;
    sem_t               empty_array;
    list<message*>      message_array;

public:
    unbounded_array();
    ~unbounded_array();
    void enqueue(message* msg); 
    message* dequeue(void);
};

class producer : public thread_run
{
    int                 prod_id;
    size_t              array_size;
    size_t              number_of_reports;
    bounded_array       message_array;
    int                 sport_count;
    int                 weather_count;
    int                 news_count;

    message* create_new_message(int terminate = 0);

public:
    producer(int id, size_t num_of_reports, size_t array_sizes);
    ~producer();

    void run();
    message *get_message(void) { return message_array.dequeue(); }
};

class dispatcher : public thread_run
{
    size_t              prod_arrays_size;
    size_t              active_arrays;
    producer**          producers;
    unbounded_array     sport; 
    unbounded_array     weather;
    unbounded_array     news;

    int dispatch_message(producer* prod);

public:
    dispatcher(size_t max_producers);
    ~dispatcher();
    
    void register_producer(producer *p);
    void run();
    message* get_message(news_type type);
};

class screen_manager
{
    bounded_array       output_array;

public:
    screen_manager(size_t max_messages);
    ~screen_manager();
    
    void push_msg(message *msg);
    void print_messages();
};

class co_editor : public thread_run
{
    screen_manager*     scr_mgr;
    dispatcher*         dspt;
    news_type           type;

public:
    co_editor(screen_manager* sm, dispatcher* d, news_type nt);
    ~co_editor();

    void run();
};
