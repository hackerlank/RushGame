local DisConnMsg = MsgTbl.DisConnMsg

local TestLoginCase = TestCase("TestLogin")

function TestLoginCase:test_LogIn(case_data)
	assert(case_data.m_Conn)
	local user_name = case_data.m_UserName
	
	--ֻ���Ե�¼�Ļ�
	print(user_name,"LogInGame")
	WaitForMsg(DisConnMsg)
end

return TestLoginCase
