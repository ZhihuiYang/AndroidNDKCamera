#include "linkedQueue.h"
#include <assert.h>
#include "include/logger.h"
#include <string.h>

#ifdef DEBUG_MEM
#include "include/mymalloc.h"
#else
#include <malloc.h>
#endif

static bool createQueue(struct LinkedQueue *q)
{
    pQNode pStartNode = (pQNode) malloc(sizeof(QNode));
    assert(NULL != pStartNode);
    
    pStartNode->next = NULL;
    q->front = q->rear = pStartNode;
    pthread_mutex_init(&q->mutex, NULL);
    q->linkedQueueSize = 0;
    return true;
}

static bool enQueue(struct LinkedQueue *q, QElemType data)
{
    pQNode pNode = (pQNode) malloc(sizeof(QNode));
    if (NULL == pNode)
    {
        return false;
    }
    pNode->next = NULL;
    pNode->data = data;
    
    pthread_mutex_lock(&q->mutex);
    q->rear->next = pNode;
    q->rear = pNode;
    q->linkedQueueSize++;
    pthread_mutex_unlock(&q->mutex);
    return true;
}

static bool deQueue(struct LinkedQueue *q, QElemType *pdata)
{
    if (q->isEmpty(q))
    {
        return false;
    }
    pthread_mutex_lock(&q->mutex);
    pQNode p;
    p = q->front->next;
    if (p == NULL)
    {
        *pdata = NULL;
        pthread_mutex_unlock(&q->mutex);
        return false;
    }
    q->front->next = p->next;
    *pdata = p->data;
    if (q->rear == p)
    {
        q->rear = q->front;
    }
    q->linkedQueueSize--;
    free(p);
    pthread_mutex_unlock(&q->mutex);
    return true;
}

static bool isEmpty(struct LinkedQueue *q)
{
    pthread_mutex_lock(&q->mutex);
    if (q->front == q->rear)
    {
        pthread_mutex_unlock(&q->mutex);
        return true;
    }
    pthread_mutex_unlock(&q->mutex);
    return false;
}

void traverse(struct LinkedQueue *q)
{
    if (q->isEmpty(q))
    {
        LOG_INFO("the linked queue is empty");
    }
    else
    {
        pthread_mutex_lock(&q->mutex);
        pQNode p = q->front->next;
        while (p)
        {
            LOG_INFO("%p\n", p->data);
            p = p->next;
        }
        pthread_mutex_unlock(&q->mutex);
    }
}

int getSize(struct LinkedQueue *q)
{
    pthread_mutex_lock(&q->mutex);
    int size = q->linkedQueueSize;
    pthread_mutex_unlock(&q->mutex);
    return size;
}

static bool destoryQueue(struct LinkedQueue *q)
{
    pthread_mutex_lock(&q->mutex);
    while (!q->isEmpty(q))
    {
        pQNode p = q->front->next;
        q->front->next = p->next;
        if (q->rear == p)
        {
            q->rear = q->front;
        }
        free(p);
    }
    free(q->front);
    q->front = q->rear = NULL;
    q->linkedQueueSize = 0;
    pthread_mutex_unlock(&q->mutex);
    pthread_mutex_destroy(&q->mutex);
    return true;
}
LinkedQueue_ptr generateLinkedQueue()
{
    LinkedQueue_ptr q = NULL;
    q = (LinkedQueue_ptr) malloc(sizeof(LinkedQueue));
    memset(q, 0, sizeof(LinkedQueue));
    q->createQueue = createQueue;
    q->isEmpty = isEmpty;
    q->enQueue = enQueue;
    q->deQueue = deQueue;
    q->traverse = traverse;
    q->destoryQueue = destoryQueue;
    
    q->createQueue(q);
    return q;
}

void destoryLinkedQueue(LinkedQueue_ptr * ppq)
{
    LinkedQueue_ptr q = *ppq;
    if (NULL != q)
    {
        q->destoryQueue(q);
        free(q);
        q = NULL;
    }
}
