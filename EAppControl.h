#pragma once
#include <pro.h>
#include <QVariant>

#define COLUMN_PropertyName  0
#define COLUMN_PropertyValue 1

enum ControlType_t
{
	UnknownControl = 0,
	krnl_window,     //����
	krnl_menu,       //�˵�
	krnl_EditBox,    //�༭��
	krnl_PicBox,     //ͼƬ��
	krnl_ShapeBox,   //���ο�
	krnl_DrawPanel,  //����
	krnl_GroupBox,   //�����
	krnl_Label,      //��ǩ
	krnl_Button,     //��ť
	krnl_CheckBox,   //ѡ���
	krnl_RadioBox,   //��ѡ��
	krnl_ComboBox,   //��Ͽ�
	krnl_ListBox,    //�б��
	krnl_ChkListBox, //ѡ���б��
	krnl_HScrollBar, //���������
	krnl_VScrollBar, //���������
	krnl_ProcessBar, //������
	krnl_SliderBar,  //������
	krnl_Tab,        //ѡ���
	krnl_AnimateBox, //Ӱ���
	krnl_DatePicker, //���ڿ�
	krnl_MonthCalendar,  //����
	krnl_DriverBox,  //��������
	krnl_DirBox,     //Ŀ¼��
	krnl_FileBox,    //�ļ���
	krnl_ColorPicker, //��ɫѡ����
	krnl_HyperLinker, //����������
	krnl_Spin,        //������
	krnl_CommonDlg,   //ͨ�öԻ���
	krnl_Timer,       //ʱ��
	krnl_printer,     //��ӡ��
	krnl_UDP,         //���ݱ�
	krnl_Client,      //�ͻ�
	krnl_Server,      //������
	krnl_SerialPort,  //�˿�
	krnl_Grid,        //���
	krnl_DataSrc,     //����Դ
	krnl_NProvider,   //ͨ���ṩ��
	krnl_DBProvider,  //���ݿ��ṩ��
	krnl_PicBtn,      //ͼ�ΰ�ť
	krnl_ODBCDB,      //�ⲿ���ݿ�
	krnl_ODBCProvider,//�ⲿ�����ṩ��
	krnl_DropTarget,  //�ϷŶ���
};


class ControlInfoWidget;
class EAppControl
{
public:
	static qstring ȡ�¼�����(ControlType_t type, int eventIndex);
	//static void ȡ�ؼ�����(ea_t propertyAddr, int propertySize, QMap<QString, QVariant>* out_Data);

	static void ��ʾ�ؼ���Ϣ(ControlType_t type, unsigned int propertyAddr, int propertySize);
	static void ����ı��ؼ�(ControlInfoWidget* pWindow, QString PropertyName, QString PropertyValue);
	static void ��Ӳ����ؼ�(ControlInfoWidget* pWindow, QString PropertyName, bool ProperyValue);
	static void ����б�ؼ�(ControlInfoWidget* pWindow, QString PropertyName, unsigned int PropertyValue);
	static void ������ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue);
	static void ���ͼƬ�ؼ�(ControlInfoWidget* pWindow, QString PropertyName, QByteArray& PropertyValue);
};