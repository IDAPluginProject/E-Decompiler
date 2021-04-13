#include "EControl.h"
#include <QMap>
#include "krnl_window.h"
#include "krnl_Button.h"
#include "krnl_Timer.h"
#include "krnl_ListBox.h"
#include "krnl_RadioBox.h"
#include "krnl_PicBtn.h"
#include "krnl_EditBox.h"
#include "krnl_PicBox.h"
#include "krnl_ShapeBox.h"
#include "krnl_CheckBox.h"
#include "krnl_DropTarget.h"

qstring EControl::ȡ�¼�����(ControlType_t type,int eventIndex)
{
	qstring ret;

	static QMap<ControlType_t, qstring(*)(int)> map_eventFunc;
	if (!map_eventFunc.size()) {
		map_eventFunc[krnl_window] = krnl_window::ȡ�¼�����;
		map_eventFunc[krnl_EditBox] = krnl_EditBox::ȡ�¼�����;
		map_eventFunc[krnl_PicBox] = krnl_PicBox::ȡ�¼�����;
		map_eventFunc[krnl_ShapeBox] = krnl_ShapeBox::ȡ�¼�����;
		map_eventFunc[krnl_CheckBox] = krnl_CheckBox::ȡ�¼�����;
		map_eventFunc[krnl_Button] = krnl_Button::ȡ�¼�����;
		map_eventFunc[krnl_Timer] = krnl_Timer::ȡ�¼�����;
		map_eventFunc[krnl_ListBox] = krnl_ListBox::ȡ�¼�����;
		map_eventFunc[krnl_RadioBox] = krnl_RadioBox::ȡ�¼�����;
		map_eventFunc[krnl_PicBtn] = krnl_PicBtn::ȡ�¼�����;
		

		map_eventFunc[krnl_DropTarget] = krnl_DropTarget::ȡ�¼�����;
 	}

	QMap<ControlType_t, qstring(*)(int)>::iterator it = map_eventFunc.find(type);
	if (it != map_eventFunc.end()) {
		ret = it.value()(eventIndex);
	}
	return ret;
}