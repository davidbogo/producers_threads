#include "ex3.h"

dispatcher::dispatcher(size_t max_producers)
{
    prod_arrays_size = max_producers;
    active_arrays = 0;
    producers = new producer*[max_producers];
}

int dispatcher::dispatch_message(producer* prod)
{
    message* message_from_prod = prod->get_message();
    switch (message_from_prod->type) {
    case NT_SPORT:
        sport.enqueue(message_from_prod);
        break;
    case NT_WEATHER:
        weather.enqueue(message_from_prod);
        break;
    case NT_NEWS:
        news.enqueue(message_from_prod);
        break;
    default: //terminated
        return 1;
    }
    return 0;
}

void dispatcher::run()
{
    while (active_arrays != 0) {
        for (int i = 0; i < prod_arrays_size; i++) {
            if (producers[i] == NULL)
                continue;
            int listen_to_prod = dispatch_message(producers[i]);
            if (listen_to_prod) {
                // Must terminate
                delete producers[i];
                producers[i] = NULL;
                active_arrays--;
            }
        }
    }
    //inform all co-editors that there will be no further messages
    message* terminated = new message();
    terminated->message_no = 0;
    terminated->producer_no = -1;
    terminated->type = NT_TERMINATE;
    sport.enqueue(terminated);
    terminated = new message();
    terminated->message_no = 0;
    terminated->producer_no = -1;
    terminated->type = NT_TERMINATE;
    weather.enqueue(terminated);
    terminated = new message();
    terminated->message_no = 0;
    terminated->producer_no = -1;
    terminated->type = NT_TERMINATE;
    news.enqueue(terminated);
}

dispatcher::~dispatcher()
{
    for (int i = 0; i < prod_arrays_size; i++) {
        if (producers[i])
            delete producers[i];
    }
    delete [] producers;
}
