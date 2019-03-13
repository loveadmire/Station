package com.hzlz;
public class Sm4Andorid{

	static{
		System.loadLibrary("Sm4Andorid");
	}
	
	public native static int sm4_cbc_enc(byte[] data);
	public native static int sm4_cbc_dec(byte[] data);
	public native static int sm4_ecb_enc(byte[] data);
        public native static int sm4_ecb_dec(byte[] data);

}
