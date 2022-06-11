#include "ex3.h"

producer::producer(int id, size_t num_of_reports, size_t array_size)
: message_array(array_size)
{
    prod_id = id;
    number_of_reports = num_of_reports;
}

message* producer::create_new_message(int terminate)
{
    message* sent_message = new message();
    if (!terminate) {
        //pick a type at random somehow
        sent_message->message_no = sport_count; //depends on type
        sport_count++; //add to the type count
        sent_message->type = NT_SPORT; //depends on type
        sent_message->producer_no = prod_id;
    } else {
        sent_message->message_no = 0;
        sent_message->type = NT_TERMINATE;
        sent_message->producer_no = prod_id;
    }

    return sent_message;
}
 
void producer::produce()
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
