#include "TracyLfq.hpp"

namespace tracy
{

LfqBlock* LfqProducerImpl::NextBlock()
{
    LfqBlock* blk = m_queue->GetFreeBlock();
    assert( blk );
    assert( blk->next.load() == nullptr );
    blk->thread = m_thread;
    lfq.dataEnd = blk->dataEnd;
    lfq.tail = &blk->tail;
    LfqBlock* oldBlk = m_block.load();
    m_block.store( blk );
    m_queue->ReleaseBlock( oldBlk );
    return blk;
}

}
