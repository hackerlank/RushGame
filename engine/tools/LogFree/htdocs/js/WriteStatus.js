var Op = new Array();
Op[0] = "<font color='#FF0000'><b>δ����</b></font>";
Op[1] = "<font color='#066016'><b>����bug����</b></font>";
Op[2] = "<font color='#000000'><b>�Ѵ�����</b></font>";
Op[3] = "<font color='#0A14D3'><b>�۲���</b></font>";
Op[4] = "<font color='#000000'><b>�ظ�bug</b></font>";

function WriteStatus(st)
{
	document.write(Op[st]);
}

function WriteOptions(st)
{
	var src = "";
	for (var i = 0; i < 5; i++)
	{
		if (st == i)
			src = src + "<option value=" + i + " selected>" + Op[i] + "</option>";
		else
			src = src + "<option value=" + i + ">" + Op[i] + "</option>";
	}
	document.write(src);
}


function ReflushUrl(url)
{
	var obj = document.getElementById("who");
	url = url + "&who=" + obj.value;
	window.location.href = url;
}

function ReflushUrlNF(url)
{
	var obj = document.getElementById("nf");
	url = url + "&nf=" + obj.value;
	window.location.href = url;
}

function ReflushUrlVersion(url)
{
	var obj = document.getElementById("version");
	url = url + "&version=" + obj.value;
	window.location.href = url;
}
