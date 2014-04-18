--[[
	��Ϊ���ǵ�ѹ�����Ե���������
	
	1�����Զ˸�������������Ӧ����
	2���ȴ�
	3�����յ�������������Ӧ����
	4�������������ȷ�������ô������һ���������׳�����

	�������ǵ���Ҫһ�׵ȴ��������������������
--]]

--����ΪԤ�ƻ�ȴ�������ȷ��Ϣ�����Ϊ�գ���ô�κ���Ϣ�����Դ���
function WaitForMsg(...)

	local arg = {...}

	assert(coroutine.running())
	
	local msg = coroutine.yield()

	assert(type(msg) == "string")
	
	if nil == arg[1] then
		return msg
	end

	for k, v in ipairs(arg) do
		if msg == v then
			return msg
		end
	end

	--�����յ���һ��������ȷ�������Ϣ,�׳��쳣
	local want_msg = ""
	for k, v in ipairs(arg) do
		want_msg = want_msg .. v .. " "
	end
	
	--error(msg .. " is the invalid msg, the msg we want is " .. want_msg)
	print("�յ���Ϣ"..msg .. ", ���ǹ��ĵ���Ϣ��:" .. want_msg)
	WaitForMsg(...)
end

--������Ϣ
function PostMsg(co, msg)
	assert(type(msg) == "string")
	coroutine.resume(co, msg)
end

function PostConnMsg(conn, msg)
	PostMsg(conn.m_Co, msg)
end
