#ifndef		_IBASECLASS_H_
#define		_IBASECLASS_H_

struct ObjectCreateStruct;

class IBaseClass
{
public:
	// ���󴴽���׼������Ҫ��Ϸ����EngineһЩ��Ϣ�����ObjectCreateStruct
	virtual void OnPrecreate(ObjectCreateStruct& ocs, float fCreateFlag) = 0;
	// �Ƿ�ѡ�е�ͨ��
	virtual void OnSelected(bool bSelected) = 0;
};
#endif