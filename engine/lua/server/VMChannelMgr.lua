engine_require "server/DBCallbackCheck"

--��0��ʼ�����������ȼ���һ
local uAllVMNum = g_ScriptApp:GetChildVMNum() - 1
g_ChildVMNum = uAllVMNum

local VMTbl = {}

for i = 0, uAllVMNum do
	VMTbl[i] = {}
end

local Channel2VM = {}

local BLOCK = true
local UNBLOCK = false

local g_queTablePool = CQueue:new()

function GetTblFromPool()
	local table
	if g_queTablePool:empty() then
		table = {}
	else
		table = g_queTablePool:front()
		g_queTablePool:pop()
	end
	return table
end

function ReturnTblToPool(table)
	for k, v in pairs(table) do
		table[k] = nil
	end
	g_queTablePool:push(table)
end

local function MergeChannelToVM(VMNum, CallDbData)
	local MergeVM = VMTbl[VMNum]
		
	for ChannelId, v in pairs(CallDbData["channel"]) do
		local uVM = Channel2VM[ChannelId]
		local vm = VMTbl[uVM]
				
		--�ϲ���һ��vm
		if uVM ~= VMNum then
			MergeVM[ChannelId] = vm[ChannelId]
			vm[ChannelId] = nil
			Channel2VM[ChannelId] = VMNum
		end
	end
		
	return MergeVM
end

local uDispatchNum = 0

--��������channel���ǵ�һ��ʹ�õ����
local function HandleChannelWithNoUsedVM(CallDbData, ...)
	--[[
		��Щchannel�ǵ�һ��ʹ�ã�����������䵽һ��vm����			
	--]]
	local arg = {...}
		
	local uVM = uDispatchNum
	uDispatchNum = uDispatchNum + 1
	if uDispatchNum > uAllVMNum then
		uDispatchNum = 0
	end
	
	--ȡ��vm
	local vm = VMTbl[uVM]
	
	for k, ChannelId in ipairs(arg) do
		Channel2VM[ChannelId] = uVM
		
		--�Ѹ�channel����vm��Ӧ��queue����ȥ
		--��Ϊ�ǵ�һ��ʹ�ø�channel�����Զ���Ҳ�����ǵ�һ�δ���
		vm[ChannelId] = CQueue:new()
		vm[ChannelId]:push(CallDbData)
	end
	
	--�����Ƿ���������Ӧ��vm�Լ�call db������
	return UNBLOCK, uVM, CallDbData
end

--��������channel����һ��vm��ʹ����Щchannel�����
local function HandleChannelWithOneUsedVM(CallDbData, UsedVM, ...)
	--[[
		��Щchannel�����п���һ�����ǵ�һ��ʹ�ã���ʹ�õ�����ͬһ��vm����
		�������Ҳ�Ƚ��鷳����Ϊ�����ڸ�vm�����channel�Ѿ���block��
	--]]
	local arg = {...}
		
	local uVM = 0
	for uUsedVMNum, v in pairs(UsedVM) do
		uVM = uUsedVMNum
		break
	end
	
	local bBlock = false

	local vm = VMTbl[uVM]
	
	for k, ChannelId in ipairs(arg) do
		--�п���channel�ǵ�һ�μ���
		if Channel2VM[ChannelId] == nil then
			Channel2VM[ChannelId] = uVM
		end
		
		if vm[ChannelId] == nil then
			vm[ChannelId] = CQueue:new()
		end
						
		--�����vm�����channel�Ѿ���block��
		if vm[ChannelId].bBlock == BLOCK then
			bBlock = true
			--�ò����Ķ�Ӧ��channel״̬��ҪΪblock
			CallDbData["channel"][ChannelId] = BLOCK			
		end
		
		vm[ChannelId]:push(CallDbData)
	end
	
	if bBlock then
		--��vm�����channel״̬ΪBLOCK
		for k, ChannelId in ipairs(arg) do
			vm[ChannelId].bBlock = BLOCK		
		end
		--��vm�����channel״̬ΪBLOCK
		return BLOCK, 0, CallDbData
	else
		return UNBLOCK, uVM, CallDbData
	end
end

--��������channel���ж��vm��ʹ����Щchannel�����
local function HandleChannelWithMoreUsedVM(CallDbData, UsedVM, uUsedNum, ...)
	--[[
		��Щchannel����ͬ��vmʹ���ˣ�
	--]]
	local arg = {...}
	local uVM = 0
	local uSelectVM = {}
	for uUsedVMNum, v in pairs(UsedVM) do
		table.insert(uSelectVM, uUsedVMNum)
	end

	--���ѡ��һ��vm
	uVM = uSelectVM[math.random(1, uUsedNum)]
	
	local BlockTbl = {}
	local uBlock = 0
	
	for k, ChannelId in ipairs(arg) do
		--����ǵ�һ�μ��룬���Ǽ���һ��ʹ�õ�vm����
		if Channel2VM[ChannelId] == nil then
			Channel2VM[ChannelId] = uVM
		end
		
		local uNum = Channel2VM[ChannelId]
		local vm = VMTbl[uNum]
		
		if vm[ChannelId] == nil then
			vm[ChannelId] = CQueue:new()
		else
			--���һ��vm����channel���������ݣ���ô���Ǽ�¼һ��
			--ÿһ��vmֻ��Ҫ��¼һ�ξ�����
			if BlockTbl[uNum] == nil then
				uBlock = uBlock + 1
			end
			BlockTbl[uNum] = true
		end
	end

	if uBlock > 1 then
		--����ж��vm�����channel���ж������ݣ���û���ˣ��ȴ���
		for k, ChannelId in ipairs(arg) do
			local uNum = Channel2VM[ChannelId]
			local vm = VMTbl[uNum]
							
			--���vm��channel���в�Ϊ�գ������ò�����Ӧ��channel��״̬ΪBLOCK��
			if not vm[ChannelId]:empty() then
				CallDbData["channel"][ChannelId] = BLOCK
			end
			
			vm[ChannelId]:push(CallDbData)	
			vm[ChannelId].bBlock = BLOCK	
		end

		return BLOCK, 0, CallDbData				
	else
		error("Request Channel �����ڲ�ͬvm�����Ƶ����û����Ӧ�Ķ�����Ϣ")
	end
end

local function WaitForChannel(...)
	local arg = {...}
	local CallDbData = {}
	CallDbData["channel"] = {}

	local UsedVM = {}
	local uUsedNum = 0
	
	for k, ChannelId in ipairs(arg) do
		--�����ҵ���β�����channel������vm�������channel��Ӧ��vm�����ڣ��Ȳ�������
		local uVM = Channel2VM[ChannelId]
		if uVM then
			if UsedVM[uVM] == nil then
				uUsedNum = uUsedNum + 1
			end
			UsedVM[uVM] = true
		end
		CallDbData["channel"][ChannelId] = UNBLOCK
	end
			
	--��channelʹ�õ�vm�Ƿ���ͬһ��vm
	if uUsedNum == 0 then
		return HandleChannelWithNoUsedVM(CallDbData, ...)		
	elseif uUsedNum == 1 then
		return HandleChannelWithOneUsedVM(CallDbData, UsedVM, ...)
	else
		return HandleChannelWithMoreUsedVM(CallDbData, UsedVM, uUsedNum, ...)
	end
end

local t_insert = table.insert
local len = string.len
local tbl_maxn = table.maxn

local tbl_blockdata = {}
local tbl_transation = {}
local b_StatisticDBTrans  = false
if GasConfig.StatisticDBTrans == 1 then
	b_StatisticDBTrans = true
end

function RequestVM(CallFunc, filename, funcname, callback, data, errfunc, ...)
	local arg = {...}
	local uSelectVM = 1

	local bBlock
	local uNum
	local CallDbData
	
	local Channels = {}
	local tbl_single = GetTblFromPool()
	
	if b_StatisticDBTrans then
		if tbl_transation[funcname] == nil then
			tbl_transation[funcname] = {0,0,{GetProcessTime()}}
		else
			t_insert(tbl_transation[funcname][3],GetProcessTime())
		end
	end

	local argn = select("#", ...)
	for i = 1, argn do
		local v = arg[i]
		if v ~= 0 and v then
			if tbl_single[v] ~= 1 then
				if type(v) == "number" then
					t_insert(Channels, v)
				elseif type(v) == "string" then
					local length = len(v)
					if length > 0 and length <= 32 then
						local hashID = String2ID(v)
						t_insert(Channels, hashID)		
					else
						LogErr("channel ��������󳤶�32","channel:"..v)
						return
					end
				else
					error("����ȷ��channel����")
				end
			end
			tbl_single[v] = 1
		end
	end
	ReturnTblToPool(tbl_single)
	
	local tickIndex
	if g_bDBCallBackCheck then
		tickIndex = RegisterDBCallbackCheckTick(funcname, Channels)
	end
	
	bBlock, uNum, CallDbData = WaitForChannel(unpack(Channels))
	
	if bBlock then
		--���block�ˣ����Ǳ��������Ϣ����unblock֮���ڵ���
		CallDbData["CallFunc"] = CallFunc
		CallDbData["filename"] = filename
		CallDbData["funcname"] = funcname
		CallDbData["callback"] = callback
		CallDbData["data"] 	   = data
		CallDbData["errfunc"]  = errfunc
		CallDbData["tickIndex"] = tickIndex
		CallDbData["bBlock"]   = BLOCK
		CallDbData["uBlockTime"] = GetProcessTime()
		CallDbData["Channels"] = arg
		t_insert(tbl_blockdata,CallDbData)
	else
		CallFunc(uNum, filename, funcname, callback, data, errfunc, tickIndex, unpack(Channels))
	end
end

---------------------------------------------------------------------------------

local function IsBlock(CallDbData)
	for ChannelId, bBlock in pairs(CallDbData["channel"]) do
		--���calldb��Ӧ��channel���б�����ģ����Ǽ����Ȱ�
		if bBlock == BLOCK then
			return true
		end
	end
	return false
end


local function CancelBlockForDbTrans(VMNum, CallDbData)
	if CallDbData["bBlock"] == false then
		return
	end
	
	local CallFunc = CallDbData["CallFunc"]
	local filename = CallDbData["filename"]
	local funcname = CallDbData["funcname"]
	local callback = CallDbData["callback"]
	local data     = CallDbData["data"]
	local errfunc  = CallDbData["errfunc"]
	local tickIndex = CallDbData["tickIndex"]
	
	local channels = {}

	for ChannelId, v in pairs(CallDbData["channel"]) do
		table.insert(channels, ChannelId)
	end
	
	CallFunc(VMNum, filename, funcname, callback, data, errfunc, tickIndex, unpack(channels))
	
	CallDbData["bBlock"] = false
	CallDbData["uBlockTime"] = nil
	for i=1,#tbl_blockdata do
		if tbl_blockdata[i] == CallDbData then
			table.remove(tbl_blockdata,i)
		end
	end
end

local function UnblockCallDbData(CallDbData, ChannelId, VMNum)
	if CallDbData["bBlock"] == true then
		--�ò�����Ӧ��channel�Ѿ�����ʹ����
		CallDbData["channel"][ChannelId] = UNBLOCK
		
		--�������channel��block�ˣ���ô�����Ȱ�
		if IsBlock(CallDbData) then
			return
		end		
		
		--���calldb������channel�����ȵ��ˣ����Ǿ�Ҫ��ʼ������
		
		--ʹ�ñ��λص���vm��Ȼ���������vm�ĸ�channel�����ƶ������vm����
		--�����´������⼸��channel����calldb��ʱ��ͺܷ����ˣ���Ϊ��ͬһ��vm������
		local MergeVM = MergeChannelToVM(VMNum, CallDbData)
		
		--���������ǰ��block��db trans
		CancelBlockForDbTrans(VMNum, CallDbData)
		
		--[[
			��Ϊ���calldb��Ѻ���ʹ�ø�channel��calldbҲ��block�ˣ�
			�����������Ǿ�Ҫ���α���channel�Ķ��У�����ǰblock��Ҳ��������
		--]]
			
		local ChannelQueue = MergeVM[ChannelId]	
			
		--�õ���queue�ĵڶ������ݣ���Ϊ��һ�����ǲŴ����calldb
		local iter = ChannelQueue._begin._next
		while iter do
			local DbData = iter._value
			DbData["channel"][ChannelId] = UNBLOCK
			
			if IsBlock(DbData) then
				break
			end
			
			MergeChannelToVM(VMNum, DbData)
			
			CancelBlockForDbTrans(VMNum, DbData)
			
			iter = iter._next
		end

		--�����Ѿ�����������queue���У��ȴ���db trans����������
		--��ô���ǰ����queue����Ϊunblock
		if iter == nil then
			MergeVM[ChannelId].bBlock = UNBLOCK
		end
	end
end

local s_format = string.format

--[[
	ÿ��calldb�ĵ��ã�����������ص���
	��һ��calldb��vm�ص�������ʱ�������ҵ���channel��Ӧ��vm��Ȼ���ҵ���vm��Ӧ�ĸ�channel��queue
	����queue��һ��Ԫ�أ���Ϊ���Ԫ�ؾ��Ǹò�����
	Ȼ���жϸ�queue��ͷԪ�أ���ʱ����һ��calldb�����ģ�������calldb��block�ˣ�
	��Ҫ��ʼ�����ˣ��Ƚ��鷳
--]]
function OnHandleVMCallback(funcname, tickIndex, ...)
	local arg = {...}
	
	if b_StatisticDBTrans then
		if tbl_transation[funcname] ~= nil then
			tbl_transation[funcname][1] = tbl_transation[funcname][1] + 1
			assert(tbl_maxn(tbl_transation[funcname][3]) > 0)
			tbl_transation[funcname][2] = tbl_transation[funcname][2] + GetProcessTime() - tbl_transation[funcname][3][1]
			table.remove(tbl_transation[funcname][3],1)
		end
	end
	
	if g_bDBCallBackCheck then
		UnRegisterDBCallbackCheckTick(tickIndex)
	end
	
	--ÿ����һ�Σ����ü�����һ
	g_App.CallDbEndEvent.RefNum = g_App.CallDbEndEvent.RefNum - 1
	assert(g_App.CallDbEndEvent.RefNum >= 0)
	
	for k, ChannelId in ipairs(arg) do
		assert(ChannelId)
		local uVM = Channel2VM[ChannelId]
		if not uVM or uVM > uAllVMNum or uVM < 0 then
			local msg = s_format("ChannelId %s��Ӧ��vm�����ڣ�channel id: %s", tostring(ChannelId), table.concat(arg, " "))
			error(msg)
		end
		
		local vm = VMTbl[uVM]
		vm[ChannelId]:pop()
		
		if vm[ChannelId]:empty() then
			vm[ChannelId] = nil
			Channel2VM[ChannelId] = nil
		else
			--�����queueû��block����ô������
			if vm[ChannelId].bBlock == BLOCK then
				--�õ���channel��һ�ε�����
				local CallDbData = vm[ChannelId]:front()
				
				UnblockCallDbData(CallDbData, ChannelId, uVM)	
			end
		end
		
	end	
	

	if g_App.CallDbEndEvent.RefNum == 0 then
		g_App.CallDbEndEvent:Set(true)
	end
end

--��ȡÿһ��vm�������channel�ĸ���
function GetVMChannelInfo()
	local tbl = {}
	for uVM = 0, uAllVMNum do
		local vm = VMTbl[uVM]
		t_insert(tbl, s_format("VM ID: %d\n", uVM))
		for uChannelId, ChannelQue in pairs(vm) do
			local uChannelNum = ChannelQue:size()
			t_insert(tbl, s_format("%s\t%d\n", tostring(uChannelId), uChannelNum))
		end
	end
	
	return table.concat(tbl)	
end

--����ǰvm��channel�����������һ���ļ������ڷ���
function DumpVMChannelInfo()
	local str = GetVMChannelInfo()
	
	local tid = GetCurrentThreadID()	
	local filename = "VMInfo_" .. tid .. ".log"
	
	local info = CLog_CreateLog(filename, "a")
	info:Write(str)
	info:Release()
end

function GetBlockChannelList()
	local tblBlockChannel = {}
	local curTime = GetProcessTime()
	for i = 1, #tbl_blockdata do
		local blockTime = curTime - tbl_blockdata[i]["uBlockTime"]
		--Ϊ���ٸ߲���ʱ���ã��ڴ����Ĺ���ֻ�ɼ�����10�������
		if blockTime > 10000 then
			table.insert(tblBlockChannel,{tbl_blockdata[i]["Channels"],tbl_blockdata[i]["funcname"], blockTime})
		end
	end
	return tblBlockChannel
end

function StartDbTranProfiler()
	b_StatisticDBTrans = true
end

function StopDbTranProfiler()
	b_StatisticDBTrans = false
	tbl_transation = {}
end

function GetDbTranProfile()
	return tbl_transation
end
