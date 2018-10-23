#include "test.h"

int main()
{
	Ans *ans = new Ans(frameRate, mode);
	if (!ans)
	{
		LOGD("new fail !\n");
		return -1;
	}

	/*while (1)
	{
		ans->deal(in, inHB, out, outHB);
	}*/

	delete ans;
	return 0;
}
