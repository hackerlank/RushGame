#pragma once
#include "CRendererBase.h"
#include "TSingleton.h"

namespace sqr
{
	class CWireBox;

	// �ر��е��赲���߼��ߵ���Ⱦ
	class CTerrainAdhereRenderer
		: public CRendererBase
		, public Singleton<CTerrainAdhereRenderer>
	{
	public:
		CTerrainAdhereRenderer() ;
		virtual ~CTerrainAdhereRenderer();

		virtual void	Render();

		void			AddRenderWireBox( CWireBox* pWireBox);
		inline bool		isRender() { return m_isRender; };
		inline void		setIsRender(bool isRender) { m_isRender = isRender; };
		inline size_t	WireBoxCount() const { return pWireBoxList.size();};
	private:
		bool							m_isRender;
		vector<CWireBox *>				pWireBoxList;
	};
}