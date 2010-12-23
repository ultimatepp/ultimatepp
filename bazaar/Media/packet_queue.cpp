#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "mediaplayer.h"
#include "utility.h"
#include "mediaplayer_in.h"


PacketQueue::PacketQueue()
{
	Init();
}

void PacketQueue::Init()
{
	first_pkt = last_pkt = 0;
    nb_packets = size = 0;
    abort_request = false;
    mutex = 0;
    cond = 0;
}

void PacketQueue::Init(AVPacket &pkt, AVPacket *flush_pkt)
{
    Init();
    mutex = SDL_CreateMutex();
    cond = SDL_CreateCond();
    Put(&pkt, flush_pkt);
}

void PacketQueue::Flush()
{
    AVPacketList *pkt, *pkt1;

    SDL_LockMutex(mutex);
    for(pkt = first_pkt; pkt != NULL; pkt = pkt1) {
        pkt1 = pkt->next;
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);
    }
    first_pkt = last_pkt = 0;
    nb_packets = size = 0;
    SDL_UnlockMutex(mutex);
}

void PacketQueue::End()
{
    Flush();
    SDL_DestroyMutex(mutex);
    SDL_DestroyCond(cond);
}

int PacketQueue::Put(AVPacket *pkt, AVPacket *flush_pkt)
{
    AVPacketList *pkt1;

    /* duplicate the packet */
    if (pkt != flush_pkt && av_dup_packet(pkt) < 0)
        return -1;

    pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    SDL_LockMutex(mutex);

    if (!last_pkt)
        first_pkt = pkt1;
    else
        last_pkt->next = pkt1;
    last_pkt = pkt1;
    nb_packets++;
    size += pkt1->pkt.size + sizeof(*pkt1);
    /* XXX: should duplicate packet data in DV case */
    SDL_CondSignal(cond);

    SDL_UnlockMutex(mutex);
    return 0;
}

void PacketQueue::Abort()
{
    SDL_LockMutex(mutex);

    abort_request = true;

    SDL_CondSignal(cond);

    SDL_UnlockMutex(mutex);
}

// return < 0 if aborted, 0 if no packet and > 0 if packet.  
int PacketQueue::Get(AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(mutex);

    for(;;) {
        if (abort_request) {
            ret = -1;
            break;
        }

        pkt1 = first_pkt;
        if (pkt1) {
            first_pkt = pkt1->next;
            if (!first_pkt)
                last_pkt = NULL;
            nb_packets--;
            size -= pkt1->pkt.size + sizeof(*pkt1);
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else 
            SDL_CondWait(cond, mutex);
    }
    SDL_UnlockMutex(mutex);
    return ret;
}