#pragma once

/*
CCoreObjectStress
�򵥵Ķ���ϵͳ�����汣����ÿһ������director��GlobalId�����Ӧ��Connection����
��lua����ͨ������SetPostionǿ�Ƹı����λ�ã������ƶ�ʱ��aoiѹ����
*/

namespace sqr
{
	class CConnStress;
	class CGas2GacOC_Create_Still_Object;
	class CGas2GacOC_Create_Moving_Object;
	class CGas2GacOC_Destroy_Object;
	class CGas2GacOC_Set_Time_Distorted_Ratio;

	class CCoreObjectStress
	{
	public:
		static CCoreObjectStress *& MainObject();
		void OnServerCommand(CConnStress *pConn, const CGas2GacOC_Create_Still_Object* pCmd);
		void OnServerCommand(CConnStress *pConn, const CGas2GacOC_Create_Moving_Object *pCmd) {}
		void OnServerCommand(CConnStress *pConn, const CGas2GacOC_Destroy_Object *pCmd) {}
		void OnServerCommand(CConnStress *pConn, const CGas2GacOC_Set_Time_Distorted_Ratio *pCmd) {}

		static void SetBeginPosition(CConnStress* pConn, float x, float y);
		static void MoveTo(CConnStress* pConn, float x, float y);
		static void SetEndPosition(CConnStress* pConn, float x, float y);
	private:
		CCoreObjectStress();
		~CCoreObjectStress();
	};
}

