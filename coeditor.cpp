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
    message* typed_message = dspt->get_message(type);
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000;
    nanosleep(&ts, NULL); // edit
    scr_mgr->push(typed_message);
}
