#ifndef HILIVE_UTIL_H_H
#define HILIVE_UTIL_H_H

#include <pthread.h>
#include "logger.h"

#define RUNING 1
#define PAUSED 0

typedef struct _threadContext
{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int status;
} ThreadContext;

void inline ThreadContext_init(ThreadContext *context){
    ThreadContext * tCtx = context;
    tCtx->tid = -1;
    pthread_mutex_init(&tCtx->mutex, NULL);
    pthread_cond_init(&tCtx->cond, NULL);
    tCtx->status = PAUSED;
}

void inline ThreadContext_destory(ThreadContext *context){
    ThreadContext *tCtx = context;
    pthread_mutex_destroy(&tCtx->mutex);
    pthread_cond_destroy(&tCtx->cond);
    tCtx->tid = -1;
    tCtx->status = PAUSED;
}

void inline resumeThread(ThreadContext *ctx)
{
    if (NULL == ctx)
    {
        return;
    }

    if (PAUSED == ctx->status)
    {
        pthread_mutex_lock(&ctx->mutex);
        ctx->status = RUNING;
        pthread_cond_signal(&ctx->cond);
        LOG_DEBUG("resume thread");
        pthread_mutex_unlock(&ctx->mutex);
    }
    else
    {
        LOG_DEBUG("thread runs already");
    }
}

void inline waitThread(ThreadContext *ctx)
{
    pthread_mutex_lock(&ctx->mutex);
    while (!ctx->status)
    {
        pthread_cond_wait(&ctx->cond, &ctx->mutex);
    }
    pthread_mutex_unlock(&ctx->mutex);
}

void inline pauseThread(ThreadContext *ctx) {
    if (NULL == ctx) {
        return;
    }

    if (RUNING == ctx->status) {
        pthread_mutex_lock(&ctx->mutex);
        ctx->status = PAUSED;
        LOG_DEBUG("pause thread");
        pthread_mutex_unlock(&ctx->mutex);
    } else {
        LOG_DEBUG("thread pause already");
    }
}


struct ImageFormat {
    int32_t width;
    int32_t height;
    int32_t format;  // ex) YUV_420
};
#endif //HILIVE_UTIL_H_H