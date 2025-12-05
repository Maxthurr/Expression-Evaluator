#include "queue.h"

#include <stddef.h>
#include <stdlib.h>

struct queue *queue_init(void)
{
    struct queue *queue = malloc(sizeof(struct queue));
    if (!queue)
        return NULL;

    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void queue_push(struct queue *queue, struct token *token)
{
    struct list *l = malloc(sizeof(struct list));
    if (!l)
        return;

    l->token = token;
    l->next = NULL;
    queue->size++;
    if (!queue->head)
    {
        queue->head = l;
        queue->tail = l;
        return;
    }

    if (queue->size == 2)
        queue->head->next = l;
    else
        queue->tail->next = l;

    queue->tail = l;
}

struct token *queue_head(struct queue *queue)
{
    return queue->head->token;
}

void queue_pop(struct queue *queue, int free_token)
{
    if (!queue->head)
        return;

    queue->size--;

    if (queue->size == 0)
    {
        if (free_token)
            free(queue->head->token);
        free(queue->head);
        queue->head = NULL;
        queue->tail = NULL;
        return;
    }

    struct list *next = queue->head->next;
    if (free_token)
        free(queue->head->token);

    free(queue->head);
    queue->head = next;

    if (queue->size == 1)
        queue->tail = queue->head;
}

void queue_clear(struct queue *queue, int free_token)
{
    while (queue->size)
        queue_pop(queue, free_token);
}

void queue_destroy(struct queue *queue, int free_token)
{
    queue_clear(queue, free_token);
    free(queue);
}
