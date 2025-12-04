#include <stdlib.h>
#include <string.h>
#include "music.h"

MusicQueue *createMusicQueue()
{
    MusicQueue *q = (MusicQueue *)malloc(sizeof(MusicQueue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

void enqueueMusicTrack(MusicQueue *q, const char *name, int tempo)
{
    MusicNode *newNode = (MusicNode *)malloc(sizeof(MusicNode));
    strcpy(newNode->track.name, name);
    newNode->track.tempo = tempo;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

MusicTrack *getCurrentTrack(MusicQueue *q)
{
    if (q->front == NULL)
        return NULL;
    return &(q->front->track);
}

void nextTrack(MusicQueue *q)
{
    if (q->front == NULL)
        return;
    MusicNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    temp->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
    }
    else
    {
        q->rear->next = temp;
        q->rear = temp;
    }
}

void freeMusicQueue(MusicQueue *q)
{
    while (q->front != NULL)
    {
        MusicNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    free(q);
}
