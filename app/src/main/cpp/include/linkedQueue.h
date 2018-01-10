
#ifndef INCLUDE_LINKEDQUEUE_H_
#define INCLUDE_LINKEDQUEUE_H_
#include <stdatomic.h>
#include "AVFrame.h"
#include <pthread.h>

//typedef unsigned char * QElemType;
typedef void * QElemType;

typedef struct QNode
{
    QElemType data;
    struct QNode * next;
} QNode, *pQNode;

typedef struct LinkedQueue
{
    pQNode front, rear;
    pthread_mutex_t mutex;
    int linkedQueueSize;        //use atomic type for multiple thread.
    bool (*createQueue)(struct LinkedQueue *q);
    bool (*enQueue)(struct LinkedQueue *q, QElemType data);
    bool (*deQueue)(struct LinkedQueue *q, QElemType *pdata);
    bool (*isEmpty)(struct LinkedQueue *q);
    void (*traverse)(struct LinkedQueue *q);
    int (*getSize)(struct LinkedQueue *q);
    bool (*destoryQueue)(struct LinkedQueue *q);
} LinkedQueue, *LinkedQueue_ptr;

LinkedQueue_ptr generateLinkedQueue();

void destoryLinkedQueue(LinkedQueue_ptr * q);

#endif /* INCLUDE_LINKEDQUEUE_H_ */
