#include "test.h"

int main()
{
	Ans *ans = new Ans(frameRate, mode);
	if (!ans)
	{
		LOGD("new fail !\n");
		return -1;
	}

	AgcImpl *agcImpl = new AgcImpl(1, 16000, 0);
	if(!agcImpl)
	{
		LOGD("new AgcImpl fail !\n");
		return -2;
	}
	

	/*while (1)
	{
		ans->deal(in, inHB, out, outHB);
	}*/

	delete ans;
	return 0;
}
