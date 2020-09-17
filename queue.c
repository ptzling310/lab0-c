// Test Test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*ok*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *tmp;
    tmp = q->head;

    while (q->head != NULL) {
        q->head = tmp->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
        tmp = q->head;
    }
    free(q);
}

bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->next = NULL;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    // newh->next = q->head;
    // q->head = newh;
    if (!q->tail) {
        q->head = newh;
        q->tail = newh;
        newh->next = NULL;

    } else {
        newh->next = q->head;
        q->head = newh;
    }

    q->size += 1;

    return true;
}
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->next = NULL;
    newh->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));


    if (!q->tail) {
        q->head = newh;
        q->tail = newh;
        // newh->next=NULL;

    } else {
        q->tail->next = newh;
        q->tail = newh;
        // newh->next=NULL ;
    }

    q->size += 1;

    return true;
}


bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *tmp;
    tmp = q->head;
    q->head = q->head->next;

    if (sp != NULL) {
        memset(sp, 0, bufsize);
        if (tmp->value) {
            strncpy(sp, tmp->value, bufsize - 1);
        }
        // free(tmp->value);
    }
    free(tmp->value);
    free(tmp);
    q->size -= 1;

    return true;
}

int q_size(queue_t *q)
{
    if (!q)
        return 0;

    return q->size;
}


void q_reverse(queue_t *q)
{
    if (!q || q->head == NULL || q->size == 1)
        return;

    list_ele_t *cur = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *prec = cur->next;

    while (prec != NULL) {
        cur->next = prev;
        prev = cur;
        cur = prec;
        prec = prec->next;
    }
    cur->next = prev;
    q->tail = q->head;
    q->head = cur;
}


list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    // complexity not accepted
    // do not need to merge(only one list)
    if (!l1)
        return l2;
    if (!l2)
        return l1;
    /*if(strcmp(l1->value , l2->value) < 0){
       l1->next = merge(l1->next,l2);
       return l1;
    } else{
       l2->next = merge(l1,l2->next);
       return l2;
    } */

    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    if (l1)
        *tmp = l1;
    if (l2)
        *tmp = l2;
    return head;
}


list_ele_t *merge_sort(list_ele_t *head)
{
    // do not need to sort
    if (!head || !head->next) {
        return head;
    }

    // split list
    list_ele_t *h1 = head;
    list_ele_t *h2 = head->next;
    while (h2 && h2->next) {
        h1 = h1->next;
        h2 = h2->next->next;
    }

    h2 = h1->next;
    h1->next = NULL;
    h1 = head;

    // sort each list
    list_ele_t *list1 = merge_sort(h1);
    list_ele_t *list2 = merge_sort(h2);
    // merge two list in to one list
    return merge(list1, list2);
}

void q_sort(queue_t *q)
{
    // do not need to sort
    if (!q || !q->head)
        return;

    // sort
    q->head = merge_sort(q->head);

    // find nwe q->tail
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
