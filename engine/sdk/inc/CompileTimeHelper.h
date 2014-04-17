#pragma once 

namespace sqr
{

	/*
		������Щ��ͷ����������ڱ���ʱ�ڽ��е�һЩc++����
		��Ҫ��Դ��C++�����˼ά����
	*/

	template<class Sub, class Base>
	class ClassConversion
	{
	private:
		typedef char Small;
		class Big { char dummy[2]; };
		static Small Test(Base);
		static Big Test(...);
		static Sub MakeSub();
	public:
		enum { exists = sizeof( (Test(MakeSub())) ) == sizeof(Small) };
	};

	template<class T>
	class ClassConversion<T, T>
	{
	public:
		enum { exists = 1 };
	};

	template<bool> struct CompileTimeChecker{
		CompileTimeChecker(...);
	};

	template<> struct CompileTimeChecker<false>{};

	template<bool> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};


#define COMPILE_TIME_CHECK(expr, msg)\
{\
	class ERROR_##msg{};\
	void(sizeof( (CompileTimeChecker<expr>( ERROR_##msg() ) ) )); \
}

#define COMPILE_TIME_ERROR(expr) { void(sizeof( (CompileTimeError<expr>()) )); }

//#define ASSERT_CLASS_DEFINED(_class) { void(sizeof( (CompileTimeError< (sizeof(_class) != 0) >()) ));};

#define ASSERT_CLASS_DEFINED(_class) COMPILE_TIME_CHECK((sizeof(_class) != 0), Class_Must_Defined_First);

}
