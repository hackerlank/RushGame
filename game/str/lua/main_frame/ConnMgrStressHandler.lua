gac_gas_require "framework/rpc/ConnMgrHandler"

local ConnSucMsg = MsgTbl.ConnSucMsg
local ConnFailedMsg = MsgTbl.ConnFailedMsg
local DisConnMsg = MsgTbl.DisConnMsg
local ConnErrorMsg = MsgTbl.ConnErrorMsg

CScriptConnMgrStressHandler = class(IScriptConnMgrStressHandler, CConnMgrHandler)

function IScriptConnMgrStressHandler:OnConnected(Conn)
	--print("OnConnect")
	PostConnMsg(Conn, ConnSucMsg)
end

function IScriptConnMgrStressHandler:OnError( Connection, sError )
	--print( "Connection " , Connection , " has error " .. sError )
	PostConnMsg(Conn, ConnErrorMsg)
end

--ֻ��Զ�̶Ͽ�����ʱ�Żص�
function IScriptConnMgrStressHandler:OnDisconnect(Conn)
	--print("OnDisConnect")
	PostConnMsg(Conn, DisConnMsg)	
end


function IScriptConnMgrStressHandler:OnConnectFailed(Conn)
	PostConnMsg(Conn, ConnFailedMsg)
end
