#!/bin/bash

source ./args

#ִ�кϲ�
baseindex=${1}
database="ybtx_$baseindex"


echo "RenameUniqueKey.sql" >> "$CombineLogFile" 2>&1
echo "�ҳ�ybtx��$database���ظ�����Ӷ���źͽ�ɫ��������������" >> "$CombineLogFile"
cat "$ExcelSqlDir"/RenameUniqueKey.sql | mysql -uroot -p$MysqlPasswd $database>> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

echo "CombineTable.sql" >> "$CombineLogFile" 2>&1
echo "�ϲ�$database�����б�ybtx" >> "$CombineLogFile"
cat "$ExcelSqlDir"/CombineTables.sql | mysql -uroot -p$MysqlPasswd $database >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

