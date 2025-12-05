#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#include "token.h"

struct list
{
    struct token *token;
    struct list *next;
};

struct queue
{
    struct list *head;
    struct list *tail;
    size_t size;
};

/*
** Create a new queue and initialize it.
** Returns `NULL` if an error occured.
*/
struct queue *queue_init(void);

/*
** Add a new element to the queue.
*/
void queue_push(struct queue *queue, struct token *token);

/*
** Returns the head element of the queue.
*/
struct token *queue_head(struct queue *queue);

/*
** Remove the head of the queue.
*/
void queue_pop(struct queue *queue, int free_token);

/*
** Remove all elements from the queue.
** `queue` must be still usable.
*/
void queue_clear(struct queue *queue, int free_token);

/*
** Empty the queue and free `queue`.
*/
void queue_destroy(struct queue *queue, int free_token);

#endif /* ! QUEUE_H */
