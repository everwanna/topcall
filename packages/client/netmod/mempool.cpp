#include "mempool.h"
#include "mutex.h"

MemPool* MemPool::m_pInstance = NULL;

MemPool::MemPool(uint32_t blockNum)
{
    m_pLock = new MutexLock(L"NetMod_Mempool_");
    for (uint32_t i = 0; i < blockNum; ++i)
    {
        char* buf = new char[MAX_MEM_SIZE];
        Packet* pkt = new Packet(buf, MAX_MEM_SIZE);
        pkt->_type = Packet::MEM_POOL_MAX_T;
        m_pool[MAX_MEM_SIZE].push_back(pkt);

        buf = new char[MID_MEM_SIZE];
        pkt = new Packet(buf, MID_MEM_SIZE);
        pkt->_type = Packet::MEM_POOL_MID_T;
        m_pool[MID_MEM_SIZE].push_back(pkt);

        buf = new char[MIN_MEM_SIZE];
        pkt = new Packet(buf, MIN_MEM_SIZE);
        pkt->_type = Packet::MEM_POOL_MIN_T;
        m_pool[MIN_MEM_SIZE].push_back(pkt);
    }
}

MemPool::~MemPool()
{
    m_pLock->lock();
    for (mempool_t::iterator io = m_pool.begin(); io != m_pool.end(); ++io)
    {
        for (std::deque<Packet*>::iterator io2 = io->second.begin(); io2 != io->second.end(); ++io2)
        {
            if (*io2)
                delete *io2;
        }
    }
    m_pool.clear();
    m_pLock->unlock();
    delete m_pLock;
}

MemPool* MemPool::Instance()
{
    if ( NULL == m_pInstance )
    {
        m_pInstance = new MemPool(MAX_MEM_BLOCKS_NUM);
    }
    return m_pInstance;
}

void	MemPool::Release()
{
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

Packet* MemPool::newPacket(const char* data, size_t len)
{
    Packet* pkt = NULL;
    m_pLock->lock();
    if ( len <= MIN_MEM_SIZE && !m_pool[MIN_MEM_SIZE].empty() )
    {
        pkt = *m_pool[MIN_MEM_SIZE].begin();
        m_pool[MIN_MEM_SIZE].pop_front();
    }
    else if ( len <= MID_MEM_SIZE && !m_pool[MID_MEM_SIZE].empty() )
    {
        pkt = *m_pool[MID_MEM_SIZE].begin();
        m_pool[MID_MEM_SIZE].pop_front();
    }
    else if ( len <= MAX_MEM_SIZE && !m_pool[MAX_MEM_SIZE].empty() )
    {
        pkt = *m_pool[MAX_MEM_SIZE].begin();
        m_pool[MAX_MEM_SIZE].pop_front();
    }
    else
    {
        pkt = new Packet();
        pkt->_data = new char[len];
        pkt->_bufLen = len;
        pkt->_type = Packet::MEM_NEW_T;
    }
    m_pLock->unlock();
    memcpy(pkt->_data, data, len);
    pkt->_dataLen = len;
    return pkt;
}

void MemPool::freePacket(Packet* pkt)
{
    if (!pkt)
        return;

    m_pLock->lock();
    if (pkt->_type == Packet::MEM_POOL_MIN_T)
    {
        pkt->reset();
        m_pool[MIN_MEM_SIZE].push_back(pkt);
    }
    else if (pkt->_type == Packet::MEM_POOL_MID_T)
    {
        pkt->reset();
        m_pool[MID_MEM_SIZE].push_back(pkt);
    }
    else if (pkt->_type == Packet::MEM_POOL_MAX_T)
    {
        pkt->reset();
        m_pool[MAX_MEM_SIZE].push_back(pkt);
    }
    else
    {
        delete pkt;
    }
    m_pLock->unlock();
}

