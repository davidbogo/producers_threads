#include "ex3.h"

producer::producer(int id, size_t num_of_reports, size_t array_sizes)
: message_array(array_sizes)
{
    prod_id = id;
    array_size = array_sizes;
    number_of_reports = num_of_reports;
    sport_count = 0;
    weather_count = 0;
    news_count = 0;
}

message* producer::create_new_message(int terminate)
{
    message* sent_message = new message();
    if (!terminate) {
        //pick a type at random
        int rand_type = rand() % 3;
        switch (rand_type) {
        case NT_SPORT:
            sent_message->message_no = sport_count; //depends on type
            sport_count++; //add to the type count
            sent_message->type = NT_SPORT; //depends on type
            sent_message->producer_no = prod_id;
            break;
        case NT_WEATHER:
            sent_message->message_no = weather_count; //depends on type
            weather_count++; //add to the type count
            sent_message->type = NT_WEATHER; //depends on type
            sent_message->producer_no = prod_id;
            break;
        default: //news
            sent_message->message_no = news_count; //depends on type
            news_count++; //add to the type count
            sent_message->type = NT_NEWS; //depends on type
            sent_message->producer_no = prod_id;
            break;
        }
    } else {
        sent_message->message_no = 0;
        sent_message->type = NT_TERMINATE;
        sent_message->producer_no = prod_id;
    }
    return sent_message;
}
 
void producer::run()
{
    message *msg;
    do {
        msg = create_new_message();
        message_array.enqueue(msg);
        number_of_reports--;
    } while (number_of_reports > 0);
    msg = create_new_message(1 /* Termination message */);
    message_array.enqueue(msg);
}

producer::~producer()
{
}
