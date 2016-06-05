#ifndef SCHEDULER_TASK_IDS_H_INCLUDED
#define SCHEDULER_TASK_IDS_H_INCLUDED

// Unique IDs for all the tasks in the program.
typedef int32_t task_id_t;
enum
{
    TASK_ID_MAIN,
    TASK_ID_TELEM_RECEIVE,
    TASK_ID_TELEM_SEND,

    NUM_TASKS,

    TASK_ID_INVALID = -1,
};

#endif
