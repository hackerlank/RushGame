engine_require "server/thread/DbChannelMgr"
engine_require "server/thread/DataBaseInit"

local clock = os.clock
local apcall = apcall
function CoreCallDbTrans(process_data)
	local data = process_data["data"]
	local filename = process_data["filename"]
	local funcname = process_data["funcname"]
	local func = 	process_data["func"]
	local callback = process_data["callback"]
	local errfunc = process_data["errfunc"]
	local channel = process_data["channel"]
	local entry_func = process_data["entryfunc"]
	local clean_func = process_data["cleanfunc"]
	local tickIndex = process_data["tickIndex"]
	
	local function CallTransaction()
	
		local function InnerCallDb()
			if callback then
				local function HandleCallBack(...)
					--�������漸��ֻ��Ϊ���ڲ��Ե�ʱ����dbִ�еķ���ֵ
					--�Լ��鵽���ĸ�db������ɵ�
					local arg = {...}
					local argv = select("#", ...)
					
					local function CheckCallBackParam(tbl)
						for k, v in pairs(tbl) do
							if type(v) == "function" or type(v) == "thread" then
								error(filename .. " " .. funcname .. "���ظ�callback�Ĳ������ǻ�������������")		
							end
							if type(v) == "table" then
								CheckCallBackParam(v)
    						end
    						
    						if type(v) == "userdata" then
    							if getmetatable(v) == CDbResultSet then
									tbl[k] = v:GetValueTable()
								else
								    if getmetatable(v) ~= CValueTable then							     
    									error(filename .. " " .. funcname .. "���ظ�callback�Ĳ������ǻ�����CDbResultSet����CValueTable")
    								end
								end
    						end
						end
					end
					
					CheckCallBackParam(arg)
			
					OnHandleDbCallback(nil, true, callback, errfunc, unpack(arg, 1, n))							
				end
				HandleCallBack(entry_func(func, data, funcname, channel))
			else	
				assert(false, "�����пյ� callback")
			end
		end
		
		local RetryNum = 0
		
		local ret, msg
		
		local error_handler = function(err_msg)
		
			if not string.find(err_msg, "Deadlock") then
				
				local msg = string.format("FuncName: %s", funcname) 
				local LogCode = 0
		
				if next(channel) then
					local channel_msg = "  Channel ID:"
				
					for k, v in pairs(channel) do
						channel_msg = channel_msg .. " " .. v
					end
							
					LogCode = LogErr(err_msg, msg .. channel_msg)
				else
					LogCode = LogErr(err_msg, msg)
				end

				--���LogCodeΪ0������δʹ���ݴ���ʱ����Ҫdb
				if LogCode == 0 then
					db()
				end
			
			end			
			
			return err_msg
		end
		
		
		while true do
			local BeginTime = clock()
			ret, msg = xpcall( InnerCallDb, error_handler )
			
			apcall(clean_func,ret)	--ÿ�ε������ ,������������
			if ret == false then
				CancelTran()

				--���������������timeout����������3�Σ��������ʧ�ܣ���ô�������˳�
				if string.find(msg, "Deadlock") then
					RetryNum = RetryNum + 1
				
					if RetryNum > 3 then
						local RetryError = msg .. " ����3��ʧ��" 
						LogErr(RetryError)
						break
					end					
				else 
					--����Ҫ�ٴδ���ֱ���˳���
					break
				end
			else 
				--����ɹ���������������
				g_DbChannelMgr:EndTran()
			
				local EndTime = clock()
				local OffTime = EndTime - BeginTime
				if OffTime > 10 then
					local type = string.format("Db Trans ִ��ʱ�����")

					local msg = string.format("FuncName : %s , �ܺ�ʱ %d �� ", funcname, OffTime)
					if next(channel) then
						local channel_msg = " Channel ID:"
						
						for k, v in pairs(channel) do
							channel_msg = channel_msg .. " " .. v
						end
						
						msg = msg .. channel_msg
					end
	
					LogErr(type, msg)
				end

				OnHandleVMCallback(nil, funcname, tickIndex, unpack(channel))
				
				return
			end	
		
		end
		
		if ret == false then
			--����������Ǿ������̵߳���errfunc
			if errfunc then
				OnHandleDbCallback(nil, false, callback, errfunc)
			end
			
			OnHandleVMCallback(nil, funcname, tickIndex, unpack(channel))		
		end
	end

	g_DbChannelMgr:CreateQueryCoroutine(CallTransaction, funcname, unpack(channel))
end

