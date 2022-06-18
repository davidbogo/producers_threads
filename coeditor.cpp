#include "ex3.h"

co_editor::co_editor(screen_manager* sm, dispatcher* d, news_type nt)
{
    scr_mgr = sm;
    dspt = d;
    type = nt;
}

co_editor::~co_editor()
{
}

void co_editor::run()
{
    message* typed_message;
    news_type nt; 
    do {
        typed_message = dspt->get_message(type);
        if (typed_message->type != NT_TERMINATE) {
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 100000000;
            nanosleep(&ts, NULL); // edit
        }
        nt = typed_message->type;
        scr_mgr->push_msg(typed_message);
    } while (nt != NT_TERMINATE);
}
