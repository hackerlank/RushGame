// --------------------------------------------------------
// ���ƣ���Ч����������
// ˵��������Ч��ܵ����䣬�ɷ��ö������ݣ�����Чʹ��
// --------------------------------------------------------

#pragma once
//#include "TSingleton.h"
#include "CVector3.h"


class ILogicHandler// : public Singleton<ILogicHandler>
{
public:
	// �ô˺�������ʹ��Ч���ر�
	virtual float GetTerrainHeight( float xGround, float yGround )	= 0;
	/// �������������Ƿ��Ļ�������
	virtual bool IsBackGrid(const sqr::CVector3f& pos) const = 0;
};