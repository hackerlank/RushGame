#pragma once
#include "SQRControl.h"

namespace sqr
{
	class SQRChartGraphicItem;
	class SQRChartLineGraph;
	class SQRChartHistogram;
	typedef SQRChartGraphicItem* HGRAPHICITEM;

	/// @brief ͼ���ࡣĿǰ֧������ͼ����״ͼ
	class GUI_API SQRChart : public SQRControl
	{
		DYNAMIC_DECLARE_WND(SQRChart)
	public:
		enum EGraphicItemType
		{
			EGraphicItem_LineGraph = 0,
			EGraphicItem_Histogram = 1,
		};
	public:
		SQRChart();
		virtual ~SQRChart();
	public: // �ӻ���̳е��麯��
		virtual int32 Create( const WndCreateParam& param );
		virtual void DrawWndBackground();
		virtual void DrawWndText();
		virtual void OnMouseMove( uint32 nFlags, int32 x, int32 y );
	public: // �߼����
		void Clear();
		size_t Count() const;
		HGRAPHICITEM AppendGraphicItem(EGraphicItemType type);
		HGRAPHICITEM GetGraphicItem(int index);

		static bool		IsHasEventState(EventStateMask Mask);
		virtual bool	ObjHasEventState(EventStateMask Mask);
	private:
		UIVector<HGRAPHICITEM> m_graphicItemList;
	};

	class SQRChartGraphicItem : public virtual CDynamicObject
	{
		friend class SQRChart;
	protected:
		typedef float						node_type;
		typedef UIVector<node_type>		container_type;
		typedef container_type::iterator	iterator;
	protected:
		container_type m_data;				///< ����yֵ������
		bool m_visible;						///< �ɼ��Ա��
		SQRChart* m_phost;					///< ������������
	public:
		SQRChartGraphicItem(SQRChart* phost = NULL);
		virtual ~SQRChartGraphicItem();
	public:
		size_t Count()const;
		bool Empty()const;
		bool GetVisible()const;
		void SetVisible(bool visilbe);
		virtual void Clear();
		SQRChart* GetHost();
	protected:
		virtual void OnRender() {}
		virtual void OnMouseMove(uint32 hostIndex, uint32 nFlags, int32 x, int32 y );
	};

	class SQRChartLineGraph : public SQRChartGraphicItem
	{
	private:
		float m_zoomY;						///< y������ϵ��
		float m_deltaY;						///< y��ƫ��
		float m_deltaX;						///< x��ƫ��
		float m_spanX;						///< ˮƽ����ļ������
		uint32 m_color;						///< ������ɫ
	public:
		SQRChartLineGraph(SQRChart* phost = NULL);
		~SQRChartLineGraph();
	public: // ���Բ���
		void AppendData(float v);
		uint32 GetColor()const;
		void SetColor(uint32 color);
		float GetZoomY()const;
		void SetZoomY(float zoomY);
		float GetDeltaY()const;
		void SetDeltaY(float deltaY);
		float GetDeltaX()const;
		void SetDeltaX(float deltaX);
		float GetSpanX()const;
		void SetSpanX(float spanX);
	private:
		int HitCheck(int32 x, int32 y);
	protected: // �麯��
		virtual void OnRender();
		virtual void OnMouseMove(uint32 hostIndex, uint32 nFlags, int32 x, int32 y);
	protected: // �ص�����
		/// @brief ������ʾToolTips������
		virtual void OnHitPoint(int32 hostIndex, int32 x, int32 y, int index, float value);

	};
	class SQRChartHistogram : public SQRChartGraphicItem
	{
	private:
		float m_zoomY;								///< y������ϵ��
		float m_itemWidth;							///< ��״ͼ�Ļ��ƿ��
		float m_spanX;								///< ˮƽ����ļ������
		UIVector< pair<uint32, uint32> > m_color;		///< ��Ӧyֵ����ɫ���飬firstΪ�����ɫ��secondΪ������ɫ
	public:
		SQRChartHistogram(SQRChart* phost = NULL);
		virtual ~SQRChartHistogram();
	public: // ���Բ���
		/// @brief ������׷������
		/// @param colorText ���ֱ�ǩ����ɫ��0��ʾcolorFill�ķ�ɫ
		void AppendData(float v, uint32 colorFill, uint32 colorText = 0);
		float GetZoomY()const;
		void SetZoomY(float zoomY);
		float GetItemWidth()const;
		void SetItemWidth(float itemWidth);
		float GetSpanX()const;
		void SetSpanX(float spanX);
	protected: // �麯��
		virtual void OnRender();
		virtual void Clear();
	};
}