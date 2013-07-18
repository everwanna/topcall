package com.qihoo.mediasdk.audio.record;

public interface IRecordListener {
	void	onFrame(byte[] buf, int offset, int len);
}
