#include "ans_interface.h"
#include "ans.h"

static AnsImpl* sg_ansImpl = NULL;

void* ansCreate(const unsigned int sampleRate, const int mode)
{
	Ans *ans = new Ans(sampleRate, mode);
    if (!ans)
    {
        LOGD("new fail !\n");
        return NULL;
    }

	return (void*)ans;
}

void ansDestroy(void* obj)
{
	Ans *ans = (Ans*)obj;
	if(ans)
	{
		delete ans;
		ans = NULL;
	}
}

int ansDeal(void* obj, short *speechFrame, short *speechFrameHB, short *outFrame, short *outFrameHB)
{
	Ans *ans = (Ans*)obj;
	if(!ans)
	{
		return -1;
	}

	return ans->deal(speechFrame, speechFrameHB, outFrame, outFrameHB);
}


int ans(short *pcm, const int bytes)
{
	if(!sg_ansImpl)
	{
		sg_ansImpl = new AnsImpl(16000, 2);
		LOGD("new AnsImpl %p!\n", sg_ansImpl);
		return -1;
	}

	return	sg_ansImpl->deal(pcm, bytes);
}


