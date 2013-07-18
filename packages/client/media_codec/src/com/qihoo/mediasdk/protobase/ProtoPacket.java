package com.qihoo.mediasdk.protobase;

import java.nio.ByteBuffer;

public class ProtoPacket implements IProtoPacket {
	private ByteBuffer mBuffer = ByteBuffer.allocate(ProtoConst.PROTO_PACKET_SIZE);
	
	private int mLen = 0;
	private short mSvid = 0;
	private short mUri = 0;
	
	public ProtoPacket() {
		mBuffer.position(8);
	}
	
	public void setSvid(short svid) {
		mSvid = svid;
	}
	
	public void setUri(short uri) {
		this.mUri = uri;
	}
	
	public short getUri() {
		return mUri;
	}
	
	@Override
	public byte[] marshall() {		
		mLen = mBuffer.position();		
		
		mBuffer.putInt(0, mLen);
		mBuffer.putShort(4, mSvid);
		mBuffer.putShort(6, mUri);	
		
		byte[] data = new byte[mLen];		
		mBuffer.position(0);
		mBuffer.get(data);
		return data;
	}
	
	@Override
	public void marshall(ByteBuffer buf) {
		
	}

	@Override
	public void unmarshall(byte[] buf) {		
		mBuffer = ByteBuffer.wrap(buf);	
		//mBuffer.order(ByteOrder.LITTLE_ENDIAN);		
		
		this.mLen = popInt();
		this.mSvid = popShort();
		this.mUri = popShort();	
	}
	
	@Override
	public void unmarshall(ByteBuffer buf) {
		this.mBuffer = buf;
	}
	
	@Override
	public void clear() {
		mBuffer.clear();
	}
	
	public ByteBuffer getBuffer() {
		return mBuffer;
	}

	public void	pushBool(Boolean val) {
		byte b = 0;
		if( val )
			b = 1;
		
		mBuffer.put(b);
	}
	
	public Boolean popBool() {
		byte b = mBuffer.get();
		return (b==1);
	}
	
	public void pushByte(byte val) {
		mBuffer.put(val);
	}
	
	public byte popByte() {
		return mBuffer.get();
	}
	
	public void pushBytes(byte[] buf) {
		if( buf == null ) {
			mBuffer.putShort((short)0);
		} else {
			mBuffer.putShort( (short)buf.length );
			mBuffer.put(buf);
		}
	}
	
	public void pushBytes(byte[] buf, int len) {
		if( buf == null || len == 0 ) {
			mBuffer.putShort((short)0);
		} else {
			mBuffer.putShort( (short)len );
			mBuffer.put(buf, 0, len);
		}
	}
	
	public byte[] popBytes() {
		int len = mBuffer.getShort();
		byte[] buf = null;
		if( len> 0 ) {
			buf = new byte[len];
			mBuffer.get(buf);
		}
		
		return buf;
	}
	
	public void pushBytes32(byte[] buf) {
		mBuffer.putInt( buf.length );
		mBuffer.put(buf);
	}
	
	public byte[] popBytes32() {
		int len = mBuffer.getInt();
		byte[] buf = null;
		if( len> 0 ) {
			buf = new byte[len];
			mBuffer.get(buf);
		}
		
		return buf;
	}
	
	public byte[] popAll() {
		int len = mBuffer.remaining();
		byte[] buf = new byte[len];
		mBuffer.get(buf);
		return buf;
	}
	
	public void pushShort(short val) {
		mBuffer.putShort(val);
	}
	
	public short popShort() {
		return mBuffer.getShort();
	}
	
	public void	pushInt(int val) {
		mBuffer.putInt(val);
	}
	
	public int popInt() {
		return mBuffer.getInt();
	}
	
	public void pushInt64(long val) {
		mBuffer.putLong(val);
	}
	
	public long popInt64() {
		return mBuffer.getLong();
	}
	
	public void pushString16(String val) {
		if( val == null ) {
			mBuffer.putShort((short)0);
			return;
		}
		
		mBuffer.putShort( (short)val.length());
		if( val.length() > 0 ) {
			mBuffer.put(val.getBytes());
		}
	}
	
	public String popString16() {
		short len = mBuffer.getShort();
		byte[] buf = null;
		if( len > 0 ) {
			buf = new byte[len];
			mBuffer.get(buf);
			return new String(buf);
		}
		
		return "";
	}
	
	public void pushIntArray(int[] vals) {
		if( vals == null ) {
			pushInt(0);
			return;
		}
		
		int len = vals.length;
		pushInt(len);
		for( int i=0; i<len; i++ ) {
			pushInt( vals[i] );
		}
	}
	
	public int[] popIntArray() {
		int len = popInt();
		int[] vals = new int[len];
		for( int i=0; i<len; i++ ) {
			vals[i] = popInt();
		}
		
		return vals;
	}
	
	public void pushShortArray(short[] vals) {
		if( vals == null ) {
			pushInt(0);
			return;
		}
		
		int len = vals.length;
		pushInt(len);
		for( int i=0; i<len; i++ ) {
			pushShort( vals[i] );
		}
	}
	
	public short[] popShortArray() {
		int len = popInt();
		short[] vals = new short[len];
		for( int i=0; i<len; i++ ) {
			vals[i] = popShort();
		}
		
		return vals;
	}
}
