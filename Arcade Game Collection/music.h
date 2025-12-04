#ifndef MUSIC_H
#define MUSIC_H

#define MAX_MUSIC_TRACKS 5

typedef struct
{
    char name[50];
    int tempo;
} MusicTrack;

typedef struct MusicNode
{
    MusicTrack track;
    struct MusicNode *next;
} MusicNode;

typedef struct
{
    MusicNode *front;
    MusicNode *rear;
    int size;
} MusicQueue;

MusicQueue *createMusicQueue();
void enqueueMusicTrack(MusicQueue *q, const char *name, int tempo);
MusicTrack *getCurrentTrack(MusicQueue *q);
void nextTrack(MusicQueue *q);
void freeMusicQueue(MusicQueue *q);

#endif
