#pragma once



class FractalTerrain
{
public:
	FractalTerrain();
	~FractalTerrain();

	void CreateTerrian(CPaintDC *dc);
	// ��������
	void Alloc2DFractArray();
	void FreeFractArray();
	void Dump2DFractArray();
	void Fill2DFractArray(int seedValue);
	float avgDiamondVals (int i, int j, int stride, int size, int subSize);
	float avgSquareVals (int i, int j, int stride, int size);
	void SecondSmooth(float *Terrain, int iSize);
	void DisplayTerrain(CPaintDC *dc);

	// ��������
	void InitGeology();
	void SmoothTerrain();
	void DisplayGeology(CPaintDC *dc);

private:
	int m_iGridPixel;

	// ����
	int m_iSize;
	float m_fHeightScale;
	float m_fH;
	float *m_Terrain;

	// ����
	int m_iStep;
	int m_iWidth;
	int *m_Geology;
};


