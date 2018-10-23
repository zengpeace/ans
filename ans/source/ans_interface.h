#ifndef _ans_interface_h_
#define _ans_interface_h_

#ifdef __cplusplus
extern "C"
{
#endif

	void* ansCreate(const unsigned int sampleRate, const int mode);
	void ansDestroy(void* obj);
	int ansDeal(void* obj, short *speechFrame, short *speechFrameHB, short *outFrame, short *outFrameHB);

	int ans(short *pcm, const int bytes);

#ifdef __cplusplus
}
#endif

#endif



