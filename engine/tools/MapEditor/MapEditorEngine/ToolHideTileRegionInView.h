#pragma once
#include "CEventOwner.h"
#include "TSingleton.h"

namespace sqr
{
	class CLogicBrush;
	class ITexture;
	struct RenderState;
	class CToolHideTileRegionInView: public Singleton<CToolHideTileRegionInView>
	{
	public:
		typedef set<int8>					subRegIndexPool;
		typedef map<int, subRegIndexPool>	invisiblePool;

	private:
		HWND				m_hTileRegionView;

		//��ˢ
		CLogicBrush*		m_pLogicBrush;
		ITexture *			m_pTileRegionLockTexture;//����ƶ�������������
		ITexture *			m_pTileRegionSetTexture; //��ʾ������Ⱦ��������
		RenderState *		m_pRS;

		/*			---------
		|2  |3	|
		---------
		|0	|1  |
		---------
		*/

		invisiblePool		m_invisibleTileRegionIndexs;//��Щ��Ⱦ�������������˸��ݱ�־�ж�����Ұ��������Ⱦ�б�fisrt ������������ second �����򱻷ֳɵ�4����
		bool				m_bShow;

	private:
		void		 RenderOneTileRegion(const int nTileRegionIndex, const int nSubTileRegionIndex, ITexture *tex);
	public:

		CToolHideTileRegionInView();
		~CToolHideTileRegionInView();
		void OnEvent(const CMsg& msg);
		void Render();
		void Initialize(HWND hRegionView);

		void IncBrushSizeInX();
		void DecBrushSizeInX();
		void IncBrushSizeInZ();
		void DecBrushSizeInZ();

		inline HWND GetTileRegionView() const
		{
			return m_hTileRegionView;
		}

		void OperatorAddTileRegionIndex();
		void OperatorDeleteTileRegion();
		void GotoSelectTileRegion(const int nTileRegionIndex);
		void SetIsShow(bool bShow);
		CToolHideTileRegionInView::invisiblePool GetTileRegionSetIndexs() const;

		void SaveTileRegionInfo(FILE * fp);
		void SaveClientTileRegionInfo(FILE * fp);
		void LoadTileRegionInfo(FILE * fp);
		void LoadOldTileRegionInfo(FILE * fp);
		void SetIsShowTileRegionVisible(const bool b);
		void HideAllTileRegion(const bool bHide);
		void ClearAllTileRegionVisibleMark();
	};
}
