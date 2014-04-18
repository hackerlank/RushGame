function setCookie(sectionid, htmlName, serverID)
{
	var expires = new Date();
	expires.setTime(expires.getTime() + 36 * 30 * 24 * 60 * 60 * 1000);
	/*   ��ʮ������ x һ���µ��� 30 �� x һ�� 24 Сʱ
   	x һСʱ 60 �� x һ�� 60 �� x һ�� 1000 ���� */
	
	document.cookie ='section=' + sectionid + ' htm=' + htmlName + ' id=' + serverID + ' ;expires=' + expires.toGMTString();
}

function getCookie(cookieName)
{
  var cookieString = document.cookie;
  var start = cookieString.indexOf(cookieName + '=');
  // ���ϵȺŵ�ԭ���Ǳ�����ĳЩ Cookie ��ֵ����
  // �� cookieName һ�����ַ�����
  if (start == -1) // �Ҳ���
    return null;
  start += cookieName.length + 1;
  var end = cookieString.indexOf(' ', start);
  if (end == -1)
  	end = cookieString.indexOf(';', start);
  if (end == -1) return cookieString.substring(start);
  return cookieString.substring(start, end);
}

function doColor(id, color, bg) 
{
	var item;
	for(var i =1; i<=document.anchors.length; i++)
	{
		item = document.getElementById(i+'');
    		item.style.color = "#000000";

		item.style.backgroundColor = "";
	}
	if(id==null) return;
	item = document.getElementById(id+'');
	item.style.color = color;
	item.style.backgroundColor = bg;
}

function SetRightHtml()
{
	var rightHtml;
	if(getCookie('htm') == null)
		rightHtml='southern.htm';
	else
		rightHtml=getCookie('htm')+'.htm';
	top.frames['serverlist'].location.replace(rightHtml);
}

function ClickLink(id)
{
	top.location.href = document.getElementById(id+'').href;
}

function OnMouseDown(sectionid, area, obj)
{
	doColor(obj.id,'blue','red')
	setCookie(sectionid ,area, String(obj.id) )
}

function OnLoad()
{
	if(getCookie('section') == 1)
	{
		doColor(getCookie('id'), 'blue', 'red');
		ClickLink(getCookie('id'));
	}
	else
	{
		doColor(1, 'blue', 'red');
		ClickLink(1);
	}
}
