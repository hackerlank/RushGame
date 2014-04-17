#pragma once

#include "CPostProcessManager.h"

namespace sqr
{

	SmartPointer(CRenderTexture);
	SmartPointer(CPostEffect);
	SmartPointer(CPostEffectPass);
	SmartPointer(CPostEffectLoader);

	class CEditPostProcessManager :public CPostProcessManager
	{
	public:
		CEditPostProcessManager(CPostProcessManager* pPostProcessMgr);
		~CEditPostProcessManager(void);

		static CEditPostProcessManager* GetInst();

		/// ��ʼ������Ч��
		void Setup();

		/// �ͷ���Դ
		void Discard();
		/// ����ָ��Ч���Ƿ�����
		bool IsEffectEnabled(const string& name);

		/// ����/����ָ��Ч��
		void SetEffectEnabled(const string& name, bool enable);

		void DoPostProcess();

		CRenderTexture* GetDefaultRenderTarget();

		//���������ܷ��� by fyz
		bool AddEffect(CPostEffectPtr effect);
		void DelEffect(const string& name);
		bool FindEffect(const string& name);
		CPostEffectPtr GetEffect(const string& name);

		CPostProcessManager* m_pPostProcessMgr;

	};

}
