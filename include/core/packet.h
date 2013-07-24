#ifndef PACKET_H
#define PACKET_H

#include <string>

class Pack
{
public:
	Pack() {
		m_pBuf = NULL;
		m_nLen = 0;
		m_nPos = 8;

		m_nSvid = 0;
		m_nUri = 0;
	}

	Pack(short svid, short uri) {	
		m_pBuf = new char[8096];
		m_nLen = 0;
		m_nPos = 8;

		m_nSvid = svid;
		m_nUri = uri;
	}

	Pack(short svid, short uri, char* buffer) {	
		m_pBuf = buffer;
		m_nLen = 0;
		m_nPos = 8;

		m_nSvid = svid;
		m_nUri = uri;
	}

	~Pack() {
	}

public:
	void	pack() {
		m_nLen = m_nPos;	

		m_nPos = 0;
		pushInt32(m_nLen);
		pushInt16(m_nSvid);
		pushInt16(m_nUri);
		m_nPos = m_nLen;
	}

	void	reset() {
		m_nLen = 0;
		m_nPos = 8;		
	}

	void	reset(int svid, int uri) {
		m_nLen = 0;
		m_nPos = 8;		
		m_nSvid = svid;
		m_nUri = uri;
	}

	void	pushInt32(int val) {
		m_pBuf[m_nPos++] = (val&0xff000000)>>24;
		m_pBuf[m_nPos++] = (val&0xff0000)>>16;
		m_pBuf[m_nPos++] = (val&0xff00)>>8;
		m_pBuf[m_nPos++] = (val&0xff);
	}

	void	pushInt16(short val) {
		m_pBuf[m_nPos++] = (val&0xff00)>>8;
		m_pBuf[m_nPos++] = (val&0xff);
	}

	void	pushInt8(char val) {
		m_pBuf[m_nPos++] = val;		
	}

	void	pushString(const std::string& val) {
		int len = val.length();
		pushInt16(len);		

		if( len > 0 ) {
			memcpy(m_pBuf+m_nPos, val.data(), len);
			m_nPos+= len;
		}
	}

	char*	getBuf() { return m_pBuf; }
	int		getLen() { return m_nLen; }

private:
	char*		m_pBuf;
	int			m_nLen;
	int			m_nPos;

	//header:		
	short		m_nSvid;
	short		m_nUri;	
};

class Unpack
{
public:
	Unpack(const char* buf, int len) {
		m_pBuf = buf;
		m_nLen = len;
		m_nPos = 0;
	}

public:
	void	popHead() {
		m_nLen = popInt32();
		m_nSvid = popInt16();
		m_nUri = popInt16();
	}

	int		popInt32() {
		short b1 = m_pBuf[m_nPos++]&0xff;
		short b2 = m_pBuf[m_nPos++]&0xff;
		short b3 = m_pBuf[m_nPos++]&0xff;
		short b4 = m_pBuf[m_nPos++]&0xff;

		return (b1<<24)|(b2<<16)|(b3<<8)|b4;
	}

	short	popInt16() {
		short b1 = m_pBuf[m_nPos++]&0xff;
		short b2 = m_pBuf[m_nPos++]&0xff;

		return (b1<<8)|b2;
	}

	char	popInt8() {
		return m_pBuf[m_nPos++];
	}

	std::string	popString() {
		short len = popInt16();
		if( len > getRemain() )
			return "";

		std::string str = std::string(m_pBuf+m_nPos, len);
		m_nPos += len;

		return str;
	}

	int		getRemain() {
		return m_nLen - m_nPos;
	}

	const char*	getBuf() {
		return m_pBuf;
	}

	int		getLen() {
		return m_nLen;
	}

	short	getSvid() { return m_nSvid; }
	short	getUri() { return m_nUri; }

private:
	const char*	m_pBuf;
	int			m_nLen;
	int			m_nPos;

	//header:		
	int			m_nPkgLen;
	short		m_nSvid;
	short		m_nUri;	
};

struct Packet
{
	virtual ~Packet(){}
	virtual void	unmarshall(Unpack up){}
	virtual void	marshall(Pack pk){}
};

#endif

