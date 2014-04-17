#pragma once

class CMemStream;

namespace sqr
{
    struct DirectShowData;
	class ITexture;
    //������Ƶ�ࣨdirect show��
	class  CDShowMovieImp : public CBaseMovieImp
    {
    public:
        CDShowMovieImp();

        virtual ~CDShowMovieImp();
 
        // �����ڴ���Ƶ
        virtual HRESULT LoadMemMovie(const char* name);
		// ����url��Ƶ
		virtual HRESULT LoadUrlMovie(const char* name);
        // ��ȡ��Ƶ�Ŀռ��С
        void GetMovieDimensions(float &width, float &height);
		// �õ���Ƶ�Ŀ��
		float GetMovieWidth(void);
		// �õ���Ƶ�ĸ߶�
		float GetMovieHeight(void);
        // ж��
        void UnloadMovie();
        // ��ͣ
        virtual void Pause();
        // ����
        virtual void Play();
        // �ط�
        virtual void Rewind();
        // ֹͣ
        virtual void Stop();
        // �Ƿ񲥷���
        virtual bool IsPlayingMovie();
		// �Ƿ񲥷����
		virtual bool IsPlayCompleted(void);
        // ������Ƶ��ͼ
        virtual void UpdateMovieTexture();
		//�õ���ǰ����
		virtual float GetProgress(void);
		// ���
		void CleanTextureContents();
    protected:
        // ��ͼ���
        int mTexWidth;
        // ��ͼ�߶�
        int mTexHeight;
        // DirectShow�ṹ��
        DirectShowData* dsdata;
        // ˮƽ����
        bool mHorizontalMirroring;
		// �ڴ��ļ�
		CMemStream* m_pMemStream;
		// �ϴμ��ص���Ƶ�ļ�����
		std::string m_strPreName;
    };
}