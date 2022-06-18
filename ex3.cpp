#include "ex3.h"

void *run_object( void *ptr )
{
    thread_run* object;
    object = (thread_run*)ptr;
    object->run();
    return NULL;
}

/*producer_to_create* readfileprod(int fd, int* co_editor_queue)
{
    char buf[512];
    size_t buf_len = sizeof(buf);
    int offset = 0;
    int counter_of_three_lines = 1;
    int start_offset = 0;
    string producer_no;
    string num_of_messages;
    string queue_size;
    string co_editor_queue_size;
    if (!buf || buf_len == 0)
        return;
    while (offset + 1 < buf_len) {
        // we will be sending a struct of every producer
        producer_info producer_to_create = new producer_info();
        // we write down here the number of the producer
        if (counter_of_three_lines == 2) {
            for (int i = 0; i < strlen(buf); i++) {
                producer_no += buf[i];
                start_offset = strlen(buf);
            }
            producer_to_create.producer_no = stoi(producer_no);
        } else if (counter_of_three_lines == 3) {
            //we write down here the number of messages for a prod
            for (int i = start_offset; i < strlen(buf); i++) {
                num_of_messages += buf[i];
                start_offset = strlen(buf);
            }
            producer_to_create.num_of_messages = stoi(num_of_messages);
        }
        size_t bytes_read = read(fd, buf + offset, 1);
        if (bytes_read == 0)
            break;
        if (buf[offset] == '\n') {
            if (counter_of_three_lines == 3) {
                counter_of_three_lines = 1;
                for (int i = start_offset; i < strlen(buf); i++) {
                queue_size += buf[i];
                }
            producer_to_create.queue_size = stoi(queue_size);
                break;
            }
            counter_of_three_lines++;
        }// the character will later be replaced by 0
        if (buf[offset] == EOF) {
            for (int i = 0; i < strlen(buf); i++) {
                co_editor_queue_size += buf[i];
                }
            *co_editor_queue = (stoi)co_editor_queue_size;
            if (*co_editor_queue <= 0)
                    *co_editor_queue = -1;
            delete producer_to_create;
            return NULL;
        }
        offset++;
    }
    buf[offset] = 0;
    return &producer_to_create;
}*/

int main(int argc, char** argv)
{
    int co_editor_queue_size = 0;
    if (argc != 2) {
        perror("Error in: number of arguments\n");
        return -1;
    }
    list<producer_info*> information_list;
    ifstream is_file(argv[1]);
    string line;
    string num_of_prod;
    string num_of_messages;
    string queue_size;
    string empty_line;
    string co_editor_q_size;
    int num_of_params = 0;
    producer_info* prod;
    if (is_file.is_open()) {
        while(getline(is_file, line)) {
            if (num_of_params == 0) {
                num_of_params = 1;
                if (line.size() == 0)
                    continue;
            }
            switch (num_of_params) {
            case 1:
                num_of_prod = line;
                num_of_params++;
                break;
            case 2:
                num_of_messages = line;
                num_of_params++;
                break;
            case 3:
                queue_size = line;
                num_of_params++;
                break;
            default:
                // we assume an empty line here
                num_of_params = 1;
                producer_info* prod = new producer_info();
                prod->producer_no = stoi(num_of_prod);
                prod->num_of_messages = (size_t)(stoi(num_of_messages));
                prod->queue_size = (size_t)(stoi(queue_size));
                information_list.push_back(prod);
                break;
            }
        }
    }
    co_editor_q_size = num_of_prod;
    co_editor_queue_size = stoi(co_editor_q_size);
    producer** prod_array = new producer*[information_list.size()];
    list<producer_info*>::iterator it;
    int i = 0;
    for (it = information_list.begin(); it != information_list.end(); ++it) {
        producer_info* cur_prod = *it;
        producer* cur_prod_created = new producer(cur_prod->producer_no, cur_prod->num_of_messages, cur_prod->queue_size);
        prod_array[i] = cur_prod_created;
        i++;
    }
    dispatcher* only_dispatcher = new dispatcher(information_list.size());
    screen_manager* only_screen_manager = new screen_manager(co_editor_queue_size);
    co_editor* sport_editor = new co_editor(only_screen_manager, only_dispatcher, NT_SPORT);
    co_editor* weather_editor = new co_editor(only_screen_manager, only_dispatcher, NT_WEATHER);
    co_editor* news_editor = new co_editor(only_screen_manager, only_dispatcher, NT_NEWS);
    for (int i = 0; i < information_list.size(); i++) {
        only_dispatcher->register_producer(prod_array[i]);
    }
    pthread_t* prod_threads = new pthread_t[information_list.size()];
    //int* thread_int = new int[information_list.size()];
    int threadret;
    for (i = 0; i < information_list.size(); i++) {
        threadret = pthread_create(&prod_threads[i], NULL, run_object, (void*)prod_array[i]);
        if (threadret != 0)
           printf("Create thread failed with error %d\n", threadret);
    }
    pthread_t dispatcher_thread, sports_thread, weather_thread, news_thread;
    int dispatch = pthread_create( &dispatcher_thread, NULL, run_object, (void*) only_dispatcher);
    int sport = pthread_create( &sports_thread, NULL, run_object, (void*) sport_editor);
    int weather = pthread_create( &weather_thread, NULL, run_object, (void*) weather_editor);
    int news = pthread_create( &news_thread, NULL, run_object, (void*) news_editor);
    only_screen_manager->print_messages();
    pthread_join(sports_thread, NULL);
    pthread_join(weather_thread, NULL);
    pthread_join(news_thread, NULL);
    pthread_join(dispatcher_thread, NULL);
    pthread_join(prod_threads[0], NULL);
    delete only_screen_manager;
    delete news_editor;
    delete weather_editor;
    delete sport_editor;
    delete only_dispatcher;
    delete [] prod_threads;
    while (information_list.size() > 0) {
        producer_info *cur_info = information_list.back();
        information_list.pop_back();
        delete cur_info;
    }
    information_list.clear();
    delete [] prod_array;
}
