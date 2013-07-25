#include "msg.h"

Msg::Msg() {
	m_pData  = new char[MSG_SIZE];
	m_nCapacity = MSG_SIZE;
	m_nLen = 0;
}

Msg::~Msg() {
}

void	Msg::assign(const char* buf, int len) {
	if( m_nCapacity < len ) {
		LOG(TAG_MPROXY, "Msg::assign, m_nCapacity<len, m_nCapacity=%d, len=%d", m_nCapacity, m_nLen);
		return;
	}

	memcpy(m_pData, buf, len);
	m_nLen = len;
}
