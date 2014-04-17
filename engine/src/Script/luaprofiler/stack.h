
typedef long clock_t;


// ���ݽڵ�
typedef struct tagProfData {
	sqr::uint64 MarkerFunctionLocalTime; // �������������ʱ���־
	sqr::uint64 MarkerFunctionTotalTime; // �������������ʱ���־
	char*   FileDefined;  // �����ļ�
	char*   FunctionName; // ������
	long    LineDefined;  // ����������
	long    CurrentLine;  // ��ǰ������
	double   LocalTime;    // ����GetSeconds���ȡʱ�䷵��ֵ��������ʱ��
	double   TotalTime;    // ͬ��
	struct tagProfData* next; // ����
} ProfData;


// ����
typedef struct tagProfState {
   int level; // ��ǰջ�ȼ�
   ProfData *top;
} ProfState;



//// �ڴ�ͳ��ģʽ�µĽڵ�
//typedef struct tagCountData {
//	int   exec_count;   // �ú���ִ�д���
//	char* FileDefined;  // �����ļ�
//	char* FunctionName; // ������
//	long  LineDefined;  // ����������
//	long  CurrentLine;  // ��ǰ������
//	float LocalTime;    // ����GetSeconds���ȡʱ�䷵��ֵ��������ʱ��
//	float TotalTime;    // ͬ��
//} CountData;


void StackPush(ProfState* S, ProfData r);
ProfData StackPop(ProfState* S);
ProfData* StackGet(ProfState* S, int idx);


