#!/bin/bash

function Main(){
	source ./config
	if [ -e $CombineLogFile ]
	then
        	rm -f "$CombineLogFile"
	fi

	echo -n "start at: "
	date
	date > $CombineLogFile

	declare -a dumpfilelist=($*)
	dumpfilecount=$#

	#������������������Ҫ����Ҫ�ϲ������ݿ�dump�ļ�
	if [ $dumpfilecount -lt 2 ]
	then
        	echo "usage:bash Main.sh sql1 sql2"
        	echo "ERROR: Need at least two args. args must be dumpfile" > $CombineLogFile
        	cat "$CombineLogFile"
        	exit 0
	fi

	#�������Ƿ�Ϊ�ļ����Ƿ����
	num=1
	while [ $num -le $dumpfilecount ]
	do
			dumpfile=${dumpfilelist[${num}-1]}
        	if [ ! -e "$dumpfile" ]
        	then    
               		echo "ERROR:dumpfile doesn't exist"
                	cat $CombineLogFile
                	exit 0  
        	fi      
        	let num=num+1
	done

	ImportToMysqlDB
	PresetAutokey
	InitMediDatabase
	ClearTables
	ClearSomeTables
	SaveMaxId
	UpdateAutoKey
	RenameArmycorps
	RenameCharName
	RenameTongName
	ResetSceneID
	SaveUserMsg
	CreateCombineTables
	CombineDatabase
	ResetInformation
	outfile

	echo -n "end at: "
	date
	date >> $CombineLogFile
}

function CheckError(){
	
	errstr=`grep "ERROR" $CombineLogFile`
	if [ "$errstr" != "" ]
	then
       		cat $CombineLogFile
        	exit 0
	fi
}

#��ΪҪ��ζ�ȡauto������ݣ�����ֱ�ӽ�auto_table��auto_key�浽������
function PresetAutokey(){
	echo "Predeal Auto_increment.txt to array"
	echo "Predeal Auto_increment.txt to array" >> $CombineLogFile
	auto_table_count=`cat $EtcDir/auto_increment.txt | wc -l`
    auto_table_list=(`cat $EtcDir/auto_increment.txt`)
    num=0
    while [ $num -lt $auto_table_count ]
    do
        auto_table=${auto_table_list[$num]}
        auto_key[$num]=`$MysqldumpDir/mysqldump -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $CombineDB $auto_table -d | grep 'AUTO_INCREMENT,' | cut -d'\`' -f2`
		if [ $auto_table = "tbl_item_static" ] #tbl_item_staticû��auto key��������Ҫƫ�ƣ�����ֻ���ֶ�ָ��auto key
		then
			auto_key[$num]="is_uId"
		fi
		meditbl_key[$num]=${auto_key[$num]}$auto_table
        let num=num+1
    done 
}

#����dumpfiles�����ݿ�,�����Ŀ�����ݿ�Ļ����ϺϷ����Ͳ���Ҫ����Ŀ�����ݿ⣨1�����ݣ���ֻҪ�ĳ�num=2�����ˣ�ע���������ݱ���
function ImportToMysqlDB(){
	num=1
	while [ $num -le $dumpfilecount ]
	do
		dumpfile=${dumpfilelist[${num}-1]}
		database="ybtx$num"
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		echo "import $dumpfile into database $database, this step may take over one hour, please wait..."
		echo "import $dumpfile into database $database" >> "$CombineLogFile"
		echo "drop database if exists $database; drop database if exists ${database}_log;" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		CheckError
#		sed "s/ \`ybtx/ \`$database/g" $dumpfile > sql$num
#		cat sql$num | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 &
		sed "s/ \`ybtx/ \`$database/g" $dumpfile | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 &
		CheckError
		let num=num+1
	done
	wait

}
#���ĳЩ����Ҫ�ı�
function ClearTables(){
	num=1
	while [ $num -le $dumpfilecount ]
	do
        database=ybtx$num
        if [ $num -eq 1 ]
        then
            database=$CombineDB
        fi
		echo "Clear $database some tables of no use"
		echo "Clear $database some tables of no use" >> $CombineLogFile
		clear_table_list=`cat $EtcDir/ClearTables.txt`
		for clear_table in $clear_table_list
		do
			sql="delete from $database.$clear_table;"
#			echo $sql
			echo $sql >> $CombineLogFile
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 
		done
		main_scene_list=`cat $EtcDir/MainSceneName.txt`
		sql="delete from $database.tbl_scene where 1=1"
		for main_scene in $main_scene_list
		do
			sql=$sql" and sc_sSceneName!='$main_scene'"
		done
#		echo $sql
		echo $sql >> $CombineLogFile
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		let num=num+1
	done
	CheckError

	#���log���һЩ��
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx${num}_log
		if [ $num -eq 1 ]
		then
			database=${CombineDB}_log
		fi
		echo "Clear $database some tables of no use"
		echo "Clear $database some tables of no use" >> $CombineLogFile
		clear_table_list=`cat $EtcDir/ClearLogTables.txt`
		for clear_table in $clear_table_list
		do
			sql="delete from $database.$clear_table;"
#			echo $sql
			echo $sql >> $CombineLogFile
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		done
		let num=num+1
	done
	CheckError

}


#��ʼ���м����ݿ���м��
function InitMediDatabase(){
	echo "create temporary database medi_database and tables"
	echo "create temporary database medi_database and tables" >> $CombineLogFile
	sql="drop database if exists $MediDatabase;create database $MediDatabase"
	echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
	cat $EtcDir/CreateMediTbl.sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $MediDatabase >> $CombineLogFile 2>&1

	echo "init temporary database medi_database"
	echo "init temporary database medi_database" >> $CombineLogFile
	num=1
	while [ $num -le $dumpfilecount ]
	do
        	database=ybtx$num
        	if [ $num -eq 1 ]
        	then
                	database=$CombineDB
        	fi

        	#��ʼ��tbl_id_add_count��
        	sql="insert into $MediDatabase.tbl_id_add_count(ian_sBaseName,ian_uId) values('$database',$num);"
#			echo $sql
			echo $sql >> $CombineLogFile
        	echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1

        	let num=num+1
	done
	CheckError

	num=0
	while [ $num -lt $auto_table_count ]
	do
		sql="alter table $MediDatabase.tbl_id_add_count add column ${meditbl_key[$num]} bigint unsigned default 0;"
#		echo $sql
		echo $sql >> $CombineLogFile
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		let num=num+1
	done
	CheckError
}

#�����������id�����ֵ���м��tbl_id_add_count
function SaveMaxId(){
	echo "Save max value of auto_key to tbl_id_add_count"
	echo "Save max value of auto_key to tbl_id_add_count" >> $CombineLogFile
	num=0
	while [ $num -lt $auto_table_count ]
	do
		dbnum=1
		while [ $dbnum -lt $dumpfilecount ]
		do
			database=ybtx$dbnum
			let tmpnum=dbnum+1
			nextdatabase=ybtx$tmpnum
			if [ $dbnum -eq 1 ]
			then
				database=$CombineDB
			fi
			sql="update ${MediDatabase}.tbl_id_add_count set ${meditbl_key[$num]}=${meditbl_key[$num]}+(select ifnull(max(${auto_key[$num]}),0) from $database.${auto_table_list[$num]}) where ian_sBaseName='$database';"
			
			sql=$sql"update ${MediDatabase}.tbl_id_add_count set ${meditbl_key[$num]}=${meditbl_key[$num]}+(select ifnull(max(${auto_key[$num]}),0) from ${nextdatabase}.${auto_table_list[$num]}) where ian_sBaseName='$nextdatabase';"
			echo $sql >> $CombineLogFile
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 
			CheckError
			let dbnum=dbnum+1
		done
		wait
		let num=num+1
	done
}

#�Ժ���Ĵ����ݿ��������id��ƫ�ƣ�ybtx2:max(ybtx)+max(ybtx2);ybtx3:max(ybtx)+2*max(ybtx2)+max(ybtx3)...
function UpdateAutoKey(){
	echo "Update auto_increment ID" 
	echo "Update auto_increment ID" >> $CombineLogFile
	num=0
	while [ $num -lt $auto_table_count ]
	do
		dbnum=2
		while [ $dbnum -le $dumpfilecount ]
		do
			database=ybtx$dbnum
			sql="select @tmp:=sum(${meditbl_key[$num]}) from ${MediDatabase}.tbl_id_add_count where ian_uId < $dbnum;"
			
			sql=$sql"select @addnum:=@tmp+ifnull(max(${auto_key[$num]}),0) from $database.${auto_table_list[$num]};"

			if [ ${auto_table_list[$num]} = "tbl_friends_class" ]
			then
				sql=$sql"update $database.${auto_table_list[$num]} set ${auto_key[$num]}=${auto_key[$num]}+@addnum where ${auto_key[$num]}>2;"
			else
				sql=$sql"update $database.${auto_table_list[$num]} set ${auto_key[$num]}=${auto_key[$num]}+@addnum;"
			fi
#			echo $sql
			echo $sql >> $CombineLogFile
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 &
			CheckError
			let dbnum=dbnum+1
		done
		wait
		let num=num+1
	done 
	echo "Update auto_increment ID OVER!"
}

#������Ӷ������
function RenameArmycorps(){
	echo "rename army corps"
	echo "rename army corps" >> $CombineLogFile

	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		sql="insert into $MediDatabase.tbl_army_info select $num,ac_uId,ac_sName,ac_uLevel from $database.tbl_army_corps;"
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 
		let num=num+1
	done
	CheckError
	
	cat $EtcDir/GetLastArmyName.sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $MediDatabase
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		#�����Ӻ�׺�����ݿ���+һ������,ÿ��һ�η������ּ�1
		let flag=num+2
		sql="update $database.tbl_army_corps a, $MediDatabase.tbl_army_info b set a.ac_sName=concat(a.ac_sName,'&$flag') where a.ac_sName=b.ac_sName and a.ac_uId!=b.ac_uId;"
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		let num=num+1	
	done
	CheckError
}

#��������ɫ��
function RenameCharName(){
	echo "rename char name"
	echo "rename char name" >> $CombineLogFile

	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		sql="insert into $MediDatabase.tbl_char_info (select $num,cs.cs_uId,c.c_sName,cb.cb_uLevel from $database.tbl_char_static cs, $database.tbl_char c, $database.tbl_char_basic cb where cs.cs_uId=c.cs_uId and cs.cs_uId=cb.cs_uId);"
		echo $sql
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 
		let num=num+1
	done
	CheckError

	cat $EtcDir/GetLastCharName.sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $MediDatabase
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		#�����Ӻ�׺�����ݿ���+һ������,ÿ��һ�η������ּ�1
		let flag=num+2
		sql="update $database.tbl_char a, $MediDatabase.tbl_char_info b set a.c_sName=concat(a.c_sName,'&$flag') where a.c_sName=b.c_sName and a.cs_uId!=b.cs_uId;"
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		let num=num+1	
	done
	CheckError
	
}

#������Ӷ��С����
function RenameTongName(){
	echo "rename tong name"
	echo "rename tong name" >> $CombineLogFile
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		sql="insert into $MediDatabase.tbl_tong_info select $num,t_uId,t_sName,t_uLevel from $database.tbl_tong;"
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1 
		let num=num+1
	done
	CheckError
	
	cat $EtcDir/GetLastTongName.sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $MediDatabase
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		#�����Ӻ�׺�����ݿ���+һ������
		let flag=num+2
		sql="update $database.tbl_tong a, $MediDatabase.tbl_tong_info b set a.t_sName=concat(a.t_sName,'&$flag') where a.t_sName=b.t_sName and a.t_uId!=b.t_uId;"
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		let num=num+1	
	done
	CheckError
}

#�޸�������������IDΪ��һ�����ID
function ResetSceneID(){
	
	echo "begin;" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd
	main_scene_list=`cat $EtcDir/MainSceneName.txt`
	num=2
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		for main_scene in $main_scene_list
		do
			sql="select @minscid:=ifnull(min(sc_uId),0) from $database.tbl_scene where sc_sSceneName='$main_scene';
			update $database.tbl_scene as sc,$database.tbl_char_position as cp set cp.sc_uId = @minscid where sc.sc_sSceneName = '$main_scene' and sc.sc_uId = cp.sc_uId;
			update $database.tbl_scene as sc,$database.tbl_war_zone as wz set wz.sc_uId = @minscid where sc.sc_sSceneName = '$main_scene' and sc.sc_uId = wz.sc_uId;
			delete from $database.tbl_scene where sc_sSceneName = '$main_scene' and sc_uId != @minscid;"
			echo $sql >> $CombineLogFile 
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		done
		let num=num+1
		sql="update $database.tbl_scene s, $CombineDB.tbl_scene tmp set s.sc_uId=tmp.sc_uId where s.sc_sSceneName=tmp.sc_sSceneName;"
		echo $sql >> $CombineLogFile 
		echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
	done
	CheckError
	echo "commit;" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd
}

#��¼�û���ĳЩ��Ϣ
function SaveUserMsg(){
	echo "save user message"
	echo "save user message" >> $CombineLogFile
	num=1
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		if [ $num -eq 1 ]
		then
			database=$CombineDB
		fi
		cat $EtcDir/SaveUserDataToMediTbl.sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $database >> $CombineLogFile 2>&1
	let num=num+1
	done
	CheckError
}

#��Щ����Ҫ����һ�������ݣ�����ֻҪ�Ѻ����������ݱ���ռ���
function ClearSomeTables(){
	echo "clear some tables of ybtx2,ybtx3..."
	echo "clear some tables of ybtx2,ybtx3..." >> $CombineLogFile
	num=2
	tables=`sed -n 1p $EtcDir/KeepMainDbTables.txt | cut -f1-`
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		for table in $tables
		do
			sql="delete from $database.$table"
			echo $sql >> $CombineLogFile 
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		done
		CheckError
		let num=num+1
	done

	echo "clear some tables of ybtx_log_2,ybtx_log_3..."
	echo "clear some tables of ybtx_log_2,ybtx_log_3..." >> $CombineLogFile
	num=2
	tables=`sed -n 2p $EtcDir/KeepMainDbTables.txt | cut -f1-`
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx${num}_log
		for table in $tables
		do
			sql="delete from $database.$table"
			echo $sql >> $CombineLogFile 
			echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
		done
		CheckError
		let num=num+1
	done
}


#������˳����Ҫ�ϲ��ı�,�Ⱥϲ�û����ı��ٺϲ�������ı�
function CreateCombineTables(){
	echo "create $AllTables that record combine tables in turn"
	echo "create $AllTables that record combine tables in turn" >> $CombineLogFile
	if [ -e $EtcDir/$AllTables ]
	then
		rm -f $EtcDir/$AllTables
	fi
	touch $EtcDir/$AllTables
	sql="show tables"
	tables=`echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $CombineDB | grep -v "tmp" | grep -v "Tables_in"`

	for table in $tables
	do
		flag_fk=`$MysqldumpDir/mysqldump -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $CombineDB $table -d | grep "FOREIGN KEY"`
		if [ -z "$flag_fk" ]  #Ϊ�յĻ�,��û�����
		then
			echo $table >> $EtcDir/$AllTables 
		fi
	done

	for table in $tables
	do
		flag_fk=`$MysqldumpDir/mysqldump -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $CombineDB $table -d | grep "FOREIGN KEY"`
		if [ -n "$flag_fk" ]  #��Ϊ�յĻ�,�������,�Ȱѹ��������ı��¼��ȥ
		then
			echo $flag_fk > tmp
			sed -i 's/REFERENCES/@/g' tmp
			tbl=`cat tmp | cut -d@ -f2 | cut -d'\`' -f2`
			if [ -z "`grep -w $tbl $AllTables`" ]
			then
				echo $tbl >> $AllTables
			fi
		fi
	done

	for table in $tables
	do
		if [ -z "`grep -w $table $AllTables`" ]
		then
			echo $table >> $AllTables
		fi
	done


	echo "create $AllLogTables that record combine tables in turn"
	echo "create $AllLogTables that record combine tables in turn" >> $CombineLogFile
	echo "show tables" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd $CombineLogDB | grep -v "Tables_in" > $EtcDir/$AllLogTables
}

#�ϲ����ݿ�
function CombineDatabase(){
	echo "combine game databases"
	echo "combine game databases" >> $CombineLogFile
	alltables=`cat $EtcDir/$AllTables`
	num=2
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx$num
		for table in $alltables
		do
#			echo "insert ignore into $CombineDB.$table select * from $database.$table;"
			echo "insert ignore into $CombineDB.$table select * from $database.$table;" >> $CombineLogFile
			echo "insert ignore into $CombineDB.$table select * from $database.$table;" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
			CheckError
		done
		let num=num+1
	done

	echo "combine game_log database"
	echo "combine game_log database" >> $CombineLogFile
	alltables=`cat $EtcDir/$AllLogTables`
	num=2
	while [ $num -le $dumpfilecount ]
	do
		database=ybtx${num}_log
		for table in $alltables
		do
#			echo "insert ignore into $CombineLogDB.$table select * from $database.$table;"
			echo "insert ignore into $CombineLogDB.$table select * from $database.$table;" >> $CombineLogFile
			echo "insert ignore into $CombineLogDB.$table select * from $database.$table;" | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
			CheckError
		done
		let num=num+1
	done

}

#���ý�ɫ��ĳЩ��Ϣ
function ResetInformation(){
	echo "reset char time and user message"
	echo "reset char time and user message" >> $CombineLogFile


	sql="update $CombineDB.tbl_user_basic as ub, $CombineDB.tbl_user_static as us, $MediDatabase.tbl_user_info as tmp 
	set ub.ub_uOnlineTime = tmp.ub_uOnlineTime,ub.ub_uYuanBao = tmp.ub_uYuanBao,ub.ub_uAgreedVer = tmp.ub_uAgreedVer
	where us.us_uId = ub.us_uId and us.us_sName = tmp.us_sName;"
	echo $sql >> $CombineLogFile
	echo $sql | mysql -h$MysqlHost -u$MysqlUser -p$MysqlPasswd >> $CombineLogFile 2>&1
	CheckError
}

#log��Ҳ��Ҫ�ϲ��������ṩ��Ҫ������
function outfile(){
	echo "at last backup data for log db"
	echo "at last backup data for log db" >> $CombineLogFile

	echo "select * from tbl_char" | mysql -u$MysqlUser -p$MysqlPasswd -h$MysqlHost $CombineDB | sed '1d' > tbl_log_char.sql
	echo "select * from tbl_tong" | mysql -u$MysqlUser -p$MysqlPasswd -h$MysqlHost $CombineDB | sed '1d' > tbl_log_tong.sql
	echo "select i.*, 0, '' from tbl_item_static i" | mysql -u$MysqlUser -p$MysqlPasswd -h$MysqlHost $CombineDB | sed '1d' > tbl_log_item_static.sql
	echo "select * from tbl_char_static" | mysql -u$MysqlUser -p$MysqlPasswd -h$MysqlHost $CombineDB | sed '1d' > tbl_log_char_static.sql
	echo "select * from tbl_user_static" | mysql -u$MysqlUser -p$MysqlPasswd -h$MysqlHost $CombineDB | sed '1d' > tbl_log_user_static.sql
}

Main $*
