#include "stdafx.h"
#include "TestProp.h"

TestProp::TestProp(void)
{
}

//����
System::Int32^  TestProp::Region::get()
{
	return m_Region;
}
void TestProp::Region::set(System::Int32^ value)
{
}

//��ɫ
System::Drawing::Color  TestProp::FogColor::get()
{
	return m_FogColor;
}
void TestProp::FogColor::set(System::Drawing::Color value)
{}

//��ʼ
System::Single^  TestProp::Begin::get()
{
	return m_Begin;
}
void TestProp::Begin::set(System::Single^ value)
{}

//����
System::Single^  TestProp::End::get()
{
	return m_End;
}

//�ܶ�
void TestProp::End::set(System::Single^ value)
{}

System::Single^  TestProp::Density::get()
{
	return m_Density;
}

void TestProp::Density::set(System::Single^ value)
{}
