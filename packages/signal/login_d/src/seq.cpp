#include "seq.h"

Seq::Seq() 
	: m_nSeq(0)
{
}

int		Seq::current() {
	return m_nSeq;
}

int		Seq::next() {
	m_nSeq++;
	return m_nSeq;
}

