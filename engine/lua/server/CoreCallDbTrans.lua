engine_require "server/VMChannelMgr"

g_App.CallDbEndEvent = CEvent:new(false)
g_App.CallDbEndEvent.RefNum = 0

function CheckDbTrans(filename, funcname)
	assert(type(filename) == "string" and type(funcname) == "string")
	 
	if g_App.m_bCallDbHalted and g_App.CallDbEndEvent.RefNum == 0 then
		--����߼��������quit����ô���ֵ��Ϊtrue���Ժ����е���CallDbTrans���ᱨ����Ϊ���ݿ��Ѿ��ڹر���
		local err = "����" .. filename .. "�����ݿ��������" .. funcname .. "����DBVM�������˳������У�����������µ�Transaction����"
		error(err)
	end

	g_App.CallDbEndEvent:Set(false)
	g_App.CallDbEndEvent.RefNum = g_App.CallDbEndEvent.RefNum + 1	
end

local CheckDbTrans = CheckDbTrans

function GlobalDbTrans(filename, funcname, callback, data, errfunc, ...)
	CheckDbTrans(filename, funcname)
	
	RequestVM(CallDbTransaction, filename, funcname, callback, data, errfunc,...)
end

function MultiDbTrans(filename, funcname, callback, data, errfunc, ...)
	CheckDbTrans(filename, funcname)
	
	RequestVM(CallDbTransaction, filename, funcname, callback, data, errfunc,...)
end
