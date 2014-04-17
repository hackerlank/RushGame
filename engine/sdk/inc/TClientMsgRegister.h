#pragma once

/************************************************************************/
/*
������Ҫ�ǽ���ѹ������ʱ��ʹ�ã�������ǰ��CConnClient������ֻ����һ��
���ӣ��������ǵý���һЩ����޸ģ�ʹ��ͬʱ���ж���������Ϸ�������
���ǻ�Ҫ�������е�����Э�飬���ǿ��ǵ��������⣬һЩ����Э������ֻ��
���ܲ��ᴦ��
*/
/************************************************************************/
#include "TMsgDispatcher.h"
//#include "PrtlGas2GacObjectDef.h"
//#include "PrtlGac2GasGenericDef.h"
//#include "PrtlGas2GacGenericDef.h"

namespace sqr
{

	template<typename TraitsConcreateConn>
	class TClientMsgRegister:
		public TBaseMsgDispatcher<typename TraitsConcreateConn::ConcreteConn_t,uint8>
	{
	public:
		typedef typename TraitsConcreateConn::ConcreteConn_t ImpConn_t;
	public:
		static void RegisterMsgHandler();
	private:

		//�����Լ��������Ϣ
		template<typename CmdClass>static void IntRegisterServerCommand();

		//ע�᳡�������Ϣ������Follower����ȣ�
		template<typename CmdClass>static void IntRegisterSceneCommand();

		//�����������������Ϣ
		template<typename CmdClass>static void IntRegisterSceneMgrCommand();

		//ע��Follower���������ʱ��Ϣ
		template<typename CmdClass>static void IntRegisterFollowerFastDelayCommand();

		//ע��Follower����������ʱ��Ϣ
		template<typename CmdClass>static void IntRegisterFollowerSlowDelayCommand();

		//ע��Follower������Ϣ
		template<typename CmdClass>static void IntRegisterFollowerCommand();

		//ע��Director������Ϣ
		template<typename CmdClass>static void IntRegisterDirectorCommand();


		//public:
		//����Ľ�������̳�
		//�����Լ��������Ϣ
		template<typename CmdClass>static void RegisterServerCommand();

		//ע�᳡�������Ϣ������Follower����ȣ�
		template<typename CmdClass>static void RegisterSceneCommand();

		//�����������������Ϣ
		template<typename CmdClass>static void RegisterSceneMgrCommand();

		//ע��Follower���������ʱ��Ϣ
		template<typename CmdClass>static void RegisterFollowerFastDelayCommand();

		//ע��Follower����������ʱ��Ϣ
		template<typename CmdClass>static void RegisterFollowerSlowDelayCommand();

		//ע��Follower������Ϣ
		template<typename CmdClass>static void RegisterFollowerCommand();

		//ע��Director������Ϣ
		template<typename CmdClass>static void RegisterDirectorCommand();
	};
}
