#include "krnl_Timer.h"

qstring krnl_Timer::ȡ�¼�����(int eventIndex)
{
	qstring ret;
	switch (eventIndex)
	{
	case 0:
		ret = "�����¼�";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}
	return ret;
}