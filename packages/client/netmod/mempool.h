#ifndef _NETMOD_MEMORY_POOL_H_
#define _NETMOD_MEMORY_POOL_H_
#include "netmod.h"
#include <deque>
#include <map>

#define MAX_MEM_BLOCKS_NUM 10

enum MEM_BLOCK_SIZE
{
    MAX_MEM_SIZE = 1024,
    MID_MEM_SIZE = 512,
    MIN_MEM_SIZE = 256
};

using namespace NetMod;
class MutexLock;
class MemPool
{
public:
    MemPool(uint32_t blockNum);
    ~MemPool();

    static MemPool* Instance();
	static void Release();

public:
    Packet* newPacket(const char* data, size_t len);
    void    freePacket(Packet* pkt);

public:
    static MemPool*     m_pInstance;
    typedef std::map<uint32_t, std::deque<NetMod::Packet*> >    mempool_t;
    mempool_t           m_pool;
    MutexLock*          m_pLock;
};

#endif

