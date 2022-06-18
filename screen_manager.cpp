#include "ex3.h"
screen_manager::screen_manager(size_t max_messages)
:output_array(max_messages)
{
}

screen_manager::~screen_manager() 
{
}

void screen_manager::push_msg(message* msg)
{
    output_array.enqueue(msg);
}

void screen_manager::print_messages()
{
    int count_terminated = 0;
    //Producer 2 SPORTS 0
    string print_string;
    string producer = "Producer";
    string producer_no;
    string type;
    string num_of_type;
    message* printed_message;
    do {
        printed_message = output_array.dequeue();
        if (printed_message->type == NT_TERMINATE) {
            count_terminated++;
        } else {
            producer_no = " " + to_string(printed_message->producer_no);
            switch (printed_message->type) {
            case NT_SPORT:
                type = " SPORTS";
                break;
            case NT_WEATHER:
                type = " WEATHER";
                break;
            default: //news
                type = " NEWS";
                break;
            }
            num_of_type = " " + to_string(printed_message->message_no);
            print_string = producer + producer_no + type + num_of_type;
            cout<< print_string << endl;
        }
        delete printed_message;
    } while(count_terminated != 3);
    print_string = "Done!";
    cout<< print_string << endl;
}