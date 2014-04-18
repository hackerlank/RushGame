<?php
/*
�ͻ���		
	error_code(������)	����ͬerror_code�Ŀͻ���log
	error_title�������� �� ��ϸ��	��ʾ������ error_stack, error_content
	client_ip�������ӣ�	����client_guid��ѯ  ���ͻ���log
	server_ip�������ӣ�	����server_guid��ѯ  ������log
	conn_guid��������)	����conn_guid��ѯ    ���ͻ��˶�Ӧ������ָ������
	urs��������)	ʱ������ָ��urs��log
	role_name��������)	ʱ������ָ��role_name��log
	happened_time	
*/
function GetClientLogHTML($content)
{
/******************��ѯ���*******************
0: code
1: title
2: name
3: time
4: urs
5: role_name
6: server_ip
7: server_guid
8: client_guid
9: conn_guid
*********************************************/
	$str_code = "<a href='list_by_code_c.php?page=1&code=%s' target='_self'>%s</a>";
	$str_title = "<a href='detail_c.php?code=%s' target='_blank'>%s</a>";
	$str_client = "<a href='list_by_guid_c.php?page=1&guid=%s' target='_self'>%s</a>";
	$str_hardware_info = "<a href='show_hdware_info.php?guid=%s' target='_blank'>�鿴</a>";
	$str_server_all = "<a href='list_by_guid_s.php?page=1&guid=%s' target='_self'>�鿴���Է��������̵�log</a>";
	$str_server_conn = "<a href='list_by_conn_guid_s.php?page=1&guid=%s' target='_self'>�鿴log</a>";
	$str_user = "<a href='list_by_user_c.php?page=1&user=%s' target='_self'>%s</a>";
	$str_role = "<a href='list_by_role_c.php?page=1&role=%s' target='_self'>%s</a>";

	$str = sprintf("<table border=1>")
		.sprintf("<tr bgcolor='#9acd32'><th width='75'>%s</th><th width='200'>%s</th><th width='100'>%s</th><th width='100'>%s</th><th width='75'>%s</th><th width='50'>%s</th><th width='75'>%s</th><th width='100'>%s</th><th width='75'>%s</th><th width='150'>%s</th><th width=50>%s</th></tr>",
		"����", "����", "�ͻ���", "������", "����", "�ʻ�", "��ɫ", "����ʱ��", "Ӳ����Ϣ", "��Ϣ", "�汾");
	
	for ($i = 0; $i < count($content); ++$i)
	{
		$color = "#ffffff";
		if ($i % 2)
			$color = "#d8d8d8";

		$row = $content[$i];
		$code = $row[0];
		$title = $row[1];
		$name = $row[2];
		$time = $row[3];
		$urs = $row[4];
		$role = $row[5];
		$ip = $row[6];
		$serv_guid = $row[7];
		$clnt_guid = $row[8];
		$conn_guid = $row[9];
		$error_msg = $row[10];
		$version = $row[11];
		$str = $str.sprintf("<tr bgcolor='%s'>\n", $color)
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_code, urlencode($code), $code).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_title, urlencode($code), str_replace('%', '%%', $title)).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_client, urlencode($clnt_guid), $name).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_server_all, urlencode($serv_guid)).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_server_conn, urlencode($conn_guid)).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_user, $urs, $urs).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_role, $role, $role).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $time)
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_hardware_info, urlencode($clnt_guid)).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", str_replace('%', '%%', $error_msg))
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $version)
			.sprintf("</tr>\n");
	
	}

	$str = $str.sprintf("</table>");
	return $str;
}

/*****************************************
������		
	error_code��������)	����ͬerror_code�Ŀͻ���log
	error_title�������� �� ��ϸ��	��ʾ������ error_stack, error_content
	server_name��������)	����server_guid��ѯ������̵�����log
	client_ip��������)	����conn_guid��ѯ    ��Ӧ�ͻ���
	urs��������)	ʱ������ָ��urs��log
	role_name��������)	ʱ������ָ��role_name��log
	happened_time	

*****************************************/
function GetServerLogHTML($content)
{
/******************��ѯ���*******************
0: code
1: title
2: name
3: time
4: urs
5: role_name
6: client_ip
7: server_guid
8: conn_guid
*********************************************/
	$str_code = "<a href='list_by_code_s.php?page=1&code=%s' target='_self'>%s</a>";
	$str_title = "<a href='detail_s.php?code=%s' target='_blank'>%s</a>";
	$str_server = "<a href='list_by_guid_s.php?page=1&guid=%s' target='_self'>%s</a>";
	$str_client_conn = "<a href='list_by_conn_guid_c.php?page=1&guid=%s' target='_self'>%s</a>";
	$str_user = "<a href='list_by_user_s.php?page=1&user=%s' target='_self'>%s</a>";
	$str_role = "<a href='list_by_role_s.php?page=1&role=%s' target='_self'>%s</a>";

	$str = "";
	$str = sprintf("<table border=1>")
		.sprintf("<tr bgcolor='#9acd32'><th width='75'>%s</th><th width='150'>%s</th><th width='150'>%s</th><th width='150'>%s</th><th width='75'>%s</th><th width='70'>%s</th><th width='120'>%s</th><th width='150'>%s</th><th width=50>%s</th></tr>",
		"����", "����", "��������", "�ͻ���", "�ʻ�", "��ɫ", "����ʱ��", "������Ϣ��Ϣ", "�汾");
	
	for ($i = 0; $i < count($content); ++$i)
	{
		$color = "#ffffff";
		if ($i % 2)
			$color = "#d8d8d8";

		$row = $content[$i];

		$code = $row[0];
		$title = $row[1];
		$name = $row[2];
		$time = $row[3];
		$urs = $row[4];
		$role = $row[5];
		$ip = $row[6];
		$serv_guid = $row[7];
		$conn_guid = $row[8];
		$error_msg = $row[9];
		$version = $row[10];
		$str = $str.sprintf("<tr bgcolor='%s'>\n", $color)
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_code, urlencode($code), $code).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_title, urlencode($code), str_replace('%', '%%', $title)).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_server, urlencode($serv_guid), $name).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_client_conn, urlencode($conn_guid), $ip).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_user, $urs, $urs).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;").sprintf($str_role, $role, $role).sprintf("</td>\n")
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $time)
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", str_replace('%','%%',$error_msg))
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $version)
			.sprintf("</tr>\n");
	
	}
	$str = $str.sprintf("</table>");
	return $str;
}

function GetUserLogHTML($content)
{
/******************��ѯ���*******************
0: code
1: title
2: name
3: time
4: urs
5: role_name
6: client_ip
7: server_guid
8: conn_guid
*********************************************/
	$str_code = "<a href='list_by_code_s.php?page=1&code=%s' target='_self'>%s</a>";
	$str_title = "<a href='detail_s.php?code=%s' target='_blank'>%s</a>";
	$str_server = "<a href='list_by_guid_s.php?page=1&guid=%s' target='_self'>%s</a>";
	$str_client_conn = "<a href='list_by_conn_guid_c.php?page=1&guid=%s' target='_self'>%s</a>";
	$str_user = "<a href='list_by_user_s.php?page=1&user=%s' target='_self'>%s</a>";
	$str_role = "<a href='list_by_role_s.php?page=1&role=%s' target='_self'>%s</a>";

	$str = "";
	$str = sprintf("<table border=1 align='center'>")
		.sprintf("<tr bgcolor='#d8d8d8'><th width='350'>%s</th><th width='100'>%s</th><th width='150'>%s</th><th width='75'>%s</th><th width='75'>%s</th><th width='170'>%s</th><th width='50'>%s</th><th width=50>%s</th></tr>",
		"content", "server", "client", "account", "role", "time", "type", "version");
	
	for ($i = 0; $i < count($content); ++$i)
	{
		$color = "#ffffff";
		if ($i % 2)
			$color = "#d8d8d8";

		$row = $content[$i];

		$code = $row[0];
		$title = $row[1];
		$name = $row[2];
		$time = $row[3];
		$urs = $row[4];
		$role = $row[5];
		$ip = $row[6];
		$serv_guid = $row[7];
		$conn_guid = $row[8];
		$error_msg = $row[9];
		$version = $row[10];
		$str = $str.sprintf("<tr bgcolor='%s'>\n", $color)
			.sprintf("\t<td align='left'>&nbsp;%s</td>\n", str_replace('%','%%',$error_msg))
			.sprintf("\t<td align='center'>&nbsp;$name</td>\n")
			.sprintf("\t<td align='center'>&nbsp;$ip</td>\n")
			.sprintf("\t<td align='center'>&nbsp;$urs</td>\n")
			.sprintf("\t<td align='center'>&nbsp;$role</td>\n")
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $time)
			.sprintf("\t<td align='center'>&nbsp;$title</td>\n")
			.sprintf("\t<td align='center'>&nbsp;%s</td>\n", $version)
			.sprintf("</tr>\n");
	
	}
	$str = $str.sprintf("</table>");
	return $str;
}
function GetNetFlag($cnf)
{
	if($cnf == 1)
		return "<font color='#0A14D3'><b>����</b></font>";
	else if($cnf == 2)
		return "<font color='#066016'><b>����</b></font>";
	else if ($cnf == 3)
		return "<b>������</b>";
	else 
		return "<font color='#FF0000'><b>δ����</b></font>";
}

function GetOwner($who, $url)
{
	$qs = "select distinct error_owner from tbl_log_info order by error_owner";
	$result = mysql_query($qs);
	$str = sprintf("<select id='who' name='who' OnChange='ReflushUrl(\"$url\")'>");
	$str .= sprintf("<option value=''>�鿴ȫ��</option>");
	while ($row = mysql_fetch_row($result)){
		if ($row[0] == $who)
		{
			$str .= sprintf("<option value='%s' selected>%s</option>", $row[0], $row[0]);	
		}
		else
			$str .= sprintf("<option value='%s' >%s</option>", $row[0], $row[0]);	
	}
	$str .= "</select>";
	return $str;
}

function GetVersion($flag, $version, $url)
{
	$qs = "";
	if ($flag == "s")
		$qs = "select distinct version from tbl_server_log_common order by version desc";
	else if ($flag == "c")
		$qs = "select distinct version from tbl_client_log_common order by version desc";

	$result = mysql_query($qs);
	$str = sprintf("<select id='version' name='version' OnChange='ReflushUrlVersion(\"$url\")'>");
	$str .= sprintf("<option value=''>�鿴ȫ��</option>");
	while ($row = mysql_fetch_row($result)){
		if ($row[0] == $version)
		{
			$str .= sprintf("<option value='%s' selected>%s</option>", $row[0], $row[0]);	
		}
		else
			$str .= sprintf("<option value='%s' >%s</option>", $row[0], $row[0]);	
	}
	$str .= "</select>";
	return $str;
}

function GetNetFlagTitle($nf, $url)
{
	$tbl_place = array ("ȫ��","����", "����");
	$str = "<select id='nf' name='nf' OnChange='ReflushUrlNF(\"$url\")'>\n";
	for ($i = 0; $i < count($tbl_place); ++$i){
		if ($i == $nf)
			$str .= "<option value=$i selected>" . $tbl_place[$i] . "</option>";
		else
			$str .= "<option value=$i >" . $tbl_place[$i] . "</option>";

	}
	$str .= "</select>";
	return $str;
}

function WriteOwner($owner)
{
	if ($owner == "δָ��")
		return "<font color='#FF0000'><b>δָ��</b></font>";
	return $owner;
}

function GetStatus($status)
{
	if ($status == 0)
		return "δ����";
	if ($status == 1)
		return "����bug����";
	if ($status == 2)
		return "�Ѵ�����";
	if ($status == 3)
		return "�۲���";
}
?>
