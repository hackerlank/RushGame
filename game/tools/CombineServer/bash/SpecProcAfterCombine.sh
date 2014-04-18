#!/bin/bash

source ./args

echo "ResetUserBasicInfo.sql" >> "$CombineLogFile" 2>&1
echo "�ϲ�������������û�����" >> "$CombineLogFile"

cat "$ExcelSqlDir"/ResetUserBasicInfo.sql | mysql -uroot -p$MysqlPasswd ybtx >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

echo "ResetCharTime.sql" >> "$CombineLogFile" 2>&1
echo "���ý�ɫ��½��ɾ��ʱ��" >> "$CombineLogFile"

cat "$ExcelSqlDir"/ResetCharTime.sql | mysql -uroot -p$MysqlPasswd ybtx >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi


