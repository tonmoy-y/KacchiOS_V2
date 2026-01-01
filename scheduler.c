static int last_index = -1;

void schedule(){
    pcb_t* current = get_current_process();

    if(current != NULL){
        if(current->state == CURRENT){
            current->state = READY;
        }
    }

    pcb_t* table = get_process_table();
    int max = get_max_process();

    for(int i=1;i<=max;i++){
        int idx = (last_index+i) % max;

        if(table[idx].pid != 0 && table[idx].state == READY){
            table[idx].state = CURRENT;
            set_current_process(&table[idx]);
            last_index = idx;
            return;
        }
    }
    set_current_process(NULL);
}