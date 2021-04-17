#include "EAppControl.h"
#include "ControlInfoWidget.h"
#include <QMap>
#include "GuiParser.h"
#include "PropertyDelegate.h"
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



QString ȡ���ָ������(unsigned int index)
{
	QString ret;
	switch (index)
	{
	case 0x0:
		ret = QStringLiteral("Ĭ����");
		break;
	case 0x00007F00:
		ret = QStringLiteral("��׼��ͷ��");
		break;
	case 0x00007F03:
		ret = QStringLiteral("ʮ����");
		break;
	case 0x00007F01:
		ret = QStringLiteral("�ı��༭��");
		break;
	case 0x00007F02:
		ret = QStringLiteral("ɳ©��");
		break;
	case 0x00007F8B:
		ret = QStringLiteral("��ͷ���ʺ���");
		break;
	case 0x00007F8A:
		ret = QStringLiteral("��ͷ��ɳ©��");
		break;
	case 0x00007F88:
		ret = QStringLiteral("��ֹ����");
		break;
	case 0x00007F86:
		ret = QStringLiteral("�����ͷ��");
		break;
	case 0x00007F83:
		ret = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F85:
		ret = QStringLiteral("��<->�ϼ�ͷ��");
		break;
	case 0x00007F82:
		ret = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F84:
		ret = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F04:
		ret = QStringLiteral("���ϼ�ͷ��");
		break;
	case 0x00007F89:
		ret = QStringLiteral("����");
		break;
	case 0xFFFFFFFF:
		ret = QStringLiteral("�Զ�����");
		break;
	default:
		ret = QStringLiteral("δ֪");
		break;
	}

	return ret;
}

qstring EAppControl::ȡ�¼�����(ControlType_t type, int eventIndex)
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

void EAppControl::��ʾ�ؼ���Ϣ(ControlType_t type, unsigned int propertyAddr, int propertySize)
{
	static QMap<ControlType_t, void(*)(unsigned int, int)> map_ShowGuiFunc;
	if (!map_ShowGuiFunc.size()) {
		map_ShowGuiFunc[krnl_window] = krnl_window::��ʾ�ؼ���Ϣ;

	}

	QMap<ControlType_t, void(*)(unsigned int, int)>::iterator it = map_ShowGuiFunc.find(type);
	if (it != map_ShowGuiFunc.end()) {
		it.value()(propertyAddr, propertySize);
	}
	return;
}

void EAppControl::����ı��ؼ�(ControlInfoWidget* pWindow, QString PropertyName, QString PropertyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);

	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(PropertyName));
	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyValue, new QTableWidgetItem(PropertyValue));
}

void EAppControl::����б�ؼ�(ControlInfoWidget* pWindow, QString PropertyName, unsigned int PropertyValue)
{

}

void EAppControl::���ͼƬ�ؼ�(ControlInfoWidget* pWindow, QString PropertyName, QByteArray& PropertyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);
	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(PropertyName));

	if (PropertyValue.size()) {
		pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("������"), ui_ImageBox));
	}
	else {
		pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral(""), ui_ImageBox));
	}

	QImage image = QImage::fromData(PropertyValue);
	pWindow->ui.ControlTable->item(insertRow, COLUMN_PropertyValue)->setData(Qt::UserRole, image);
	return;
}

void EAppControl::������ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);

	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ָ��")));
	QComboBox* pComboBox = new QComboBox();
	pComboBox->setEditable(false);

	QStringList Items = {
		QStringLiteral("Ĭ����"),
		QStringLiteral("��׼��ͷ��"),
		QStringLiteral("ʮ����"),
		QStringLiteral("�ı��༭��"),
		QStringLiteral("ɳ©��"),
		QStringLiteral("��ͷ���ʺ���"),
		QStringLiteral("��ͷ��ɳ©��"),
		QStringLiteral("��ֹ����"),
		QStringLiteral("�����ͷ��"),
		QStringLiteral("��<->����ͷ��"),
		QStringLiteral("��<->�ϼ�ͷ��"),
		QStringLiteral("��<->����ͷ��"),
		QStringLiteral("��<->����ͷ��"),
		QStringLiteral("���ϼ�ͷ��"),
		QStringLiteral("����"),
		QStringLiteral("�Զ�����"),
	};
	pComboBox->addItems(Items);
	pComboBox->setCurrentText(ȡ���ָ������(ProperyValue));
	pWindow->ui.ControlTable->setCellWidget(insertRow, COLUMN_PropertyValue, pComboBox);
}

void EAppControl::��Ӳ����ؼ�(ControlInfoWidget* pWindow, QString PropertyName, bool ProperyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);

	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(PropertyName));
	QComboBox* pComboBox = new QComboBox();
	pComboBox->setEditable(false);
	pComboBox->addItem(QStringLiteral("��"));
	pComboBox->addItem(QStringLiteral("��"));
	pComboBox->setCurrentIndex(!ProperyValue);
	pWindow->ui.ControlTable->setCellWidget(insertRow, COLUMN_PropertyValue, pComboBox);
}