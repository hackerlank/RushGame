#!/usr/bin/php
<html>
<title>��������log��ϸ</title>
<head>
<script type="text/javascript" src="/js/pager.js"></script>
<script type="text/javascript" src="/js/WriteStatus.js"></script>
<script type="text/javascript"/>
<!--//
function submit_and_up_reload()
{
	var opner = window.opener
	document.form1.submit();
        if (opener)
                opener.location.reload();
}
//-->
</script>
<style>
.ta
{ 
	width:800; 
	height:500;
	word-wrap: normal;
	overflow-x:auto; 
	overflow-y:auto;
}
.tb
{ 
	width:800; 
	height:100;
	word-wrap: normal;
	overflow-x:auto; 
	overflow-y:auto;
}


</style>
</head>
<body>
<p><a href='javascript:history.back(1)'><font face=arial size=2>[����]</font></a></p>
<?php
require_once("../../conf/LogFree/config.php");
require_once("../../lib/LogFree/Lib.php");
function ViewLog()
{
/*
create table tbl_serv_log( id, error_code, error_title, 
        error_content, happened_time, urs, role_name, clientip, serverip, guid
)engine = MyIsam;
*/
	global $db_host, $db_user, $db_pass, $db_name;	
	$page = intval($_GET["page"]);
	$code = $_GET["code"];
	$peer = intval($_GET["peer"]);

	$Conn = mysql_connect($db_host, $db_user, $db_pass);
	mysql_query("SET NAMES 'GBK'");
	mysql_select_db($db_name, $Conn);
	$code = mysql_escape_string($code);
	
	$qs = "select max(version) from tbl_server_log_common where error_code = '$code'";
	$result = mysql_query($qs);
	$info = mysql_fetch_row($result);
	$version = $info[0];
	

	$qs = "select * from tbl_log_info where error_code = '$code'";
	$result = mysql_query($qs);
	$str_code = "<a href='list_by_code_s.php?page=1&code=%s' target='_self'>%s</a>";

	$str = "";
	$content = array();
	if($row=mysql_fetch_row($result))
	{
		$str = $str.sprintf("<form action='ChangeStatus.php' method='get' target='_self'>")
                        .sprintf("<b>log��ϸ���£�</b></p>\n")              
			.sprintf("<table border=1>")
                        .sprintf("<tr><td align='center' width='150'>��Դ:</td><td width='850'> ��������</td></tr>\n")
                        .sprintf("<tr><td align='center'>�����룺</td><td>%s</td></tr>\n", str_replace('%', '%%', $row[0]))
			.sprintf("<input type=hidden name='code' value='%s'/>", $row[0])
			.sprintf("<tr><td align='center'>�޸�˵��:</td><td><textarea wrap='off' class='tb' name='log_desc'>%s</textarea></td></tr>\n", str_replace('%', '%%', $row[7]))
			.sprintf("<tr><td align='center'>ָ�ɸ�: </td><td><input type='text' name='owner' value='%s'/></td></tr>", $row[5])
			.sprintf("<tr><td align='center'>����״̬:</td><td><select name='status'><script>WriteOptions(%d)</script></select>", $row[6])
			.sprintf("<input type=hidden name=peer value='s'/>")
			.sprintf("<input type=submit value='�޸�'onclick='submit_and_up_reload()'/></td></tr>\n")
                        .sprintf("<tr><td align='center'>���⣺</td><td>&nbsp;%s</td></tr>\n", str_replace('%', '%%', $row[1]))
			.sprintf("<tr><td align='center'>��ϸ��</td><td>").sprintf($str_code, $code, "����鿴������ $code ��������ϸ�б�").sprintf("</td></tr>")
                        .sprintf("<tr><td align='center'>�������ͣ�</td><td>&nbsp;%s</td></tr>\n", str_replace('%', '%%', $row[2]))
			.sprintf("<tr><td align='center'>���汾��</td><td>&nbsp;%s</td></tr>\n", $version)
			.sprintf("<tr><td align='center'>������Ϣ��</td><td>&nbsp;%s</td></tr>\n", str_replace('%', '%%',$row[4]))
                        .sprintf("<tr><td align='center'>����:</td><td><textarea wrap='off' class='ta' readonly>%s</textarea></td></tr>\n", str_replace('%','%%',$row[3]))
			.sprintf("</table></form>\n");
	}
	printf($str);
}
ViewLog();
?>
<p><a href='javascript:history.back(1)'><font face=arial size=2>[����]</font></a></p>
</body>
</html>

