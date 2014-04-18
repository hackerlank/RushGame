str_require "rpc/Gas2StrInc"
gac_gas_require "framework/rpc/RemoteInfo"
str_require "rpc/LoginRpc"
str_require "rpc/SceneRpc"
str_require "rpc/ShopRpc"

function Gas2Gac:OnError(num, fun)
--[[
		-1 ����ʾid����
		-2 ����ʾ��֤�����
		-3 ����ʾ���ݸ�ʽ����
		-4 ����ʾû���ҵ�������
		-5 ����ʾ���صĴ���
		>0 ����ʾ�û��Լ����صĲ���
--]]
	if num == -4 then
		--print(fun)
		return true
	end
	error(fun .. "rpc call error " .. tostring(num) )
	return false
end

--ѹ���ʱ��,�Ż����Щ��Ϣ֪ͨ�ͻ���
function StressRpcToServer(func, ...)
	Gac2Gas[func](Gac2Gas,...)
end
