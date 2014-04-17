CCoSync = class()

local CWaitForTick = class(CTick)

function CWaitForTick:Ctor(Info)
	assert(not IsNil(Info))
	self.m_Info = Info
end


function CWaitForTick:OnTick()
	g_App:UnRegisterTick(self)
	local co=self.m_co
	coroutine.resume( co, nil )
end


local function IsSyncObject(Object)
		local class=getmetatable(Object)
		return (Object ~= nil) and	IsTable(Object) and
		( (  class == CEvent) or ( class == CSemaphore) or ( class == CMsgQueue)	)
end


--[[
	�ȴ�һ�����߶��Object������ΪEnabled,ֱ����ʱ��
	uWaitTime:	�ȴ���ʱ�䳤�ȣ�����Ϊ��λ��nil��ʾ���õȴ���
	bAll:				true��ʾ�ȴ�����ͬ��������Ч��false��ʾ�ȴ�����һ��ͬ��������Ч
	�ɱ������	һ�����߶��ͬ�����������	
	����ֵ��    nil��ʾtimed out, ����{event1,event2,...}���Ʊ�ʾ�ĸ�event��Ч��
--]]

function CCoSync.WaitFor(uWaitTime,bAll,...)
	local arg = {...}
	assert(not IsNil(bAll) and IsBoolean(bAll))
	assert(IsNil(uWaitTime) or IsNumber(uWaitTime))
	local tObjects=arg
	
	if next(tObjects)==nil then
		--print("һ��ͬ������û��")
		return	--һ��ͬ������û��
	end
	
	--�����ͬ�������Ӧ�Ķ��в����ڣ���˵����ͬ����������Ч��
	
	local tResultObjects={}
	
	local bNeedWait=false

	if bAll then
		
		--�ȴ����е�ͬ������		
		for i,Object in ipairs(tObjects) do
			
			if not IsSyncObject(Object) then
				error("WaitFor failed:event is invalid.")
			end
			
			if not Object:IsReady() then
				--������һ����Чͬ�����������˳�ѭ������ʾ�������ȴ�
				bNeedWait=true
				break
			end
			
			table.insert(tResultObjects,Object)
			
		end
		
	else
		
		--�ȴ�����һ��ͬ������
		bNeedWait = true
		for i,Object in ipairs(tObjects) do
			
			if not IsSyncObject(Object) then
				error("WaitFor failed:event is invalid.")
			end
			
			if Object:IsReady() then
				--������һ����Чͬ�����󣬽���ͬ����������б���
				table.insert(tResultObjects,Object)	
				
				bNeedWait = false
				
			end
		end
		
		--bNeedWait=(next(tResultObjects) ~= nil)
				
	end
	
	if not bNeedWait then
		for i,v in ipairs(tResultObjects) do
			if v._Take then
				v:_Take()
			end
		end
		
		return tResultObjects
	end
		
	--������ȴ���������Ҫ����ȴ�����
	
	local co=coroutine.running()
		
	local Info=CWaitForCoInfo:new(bAll)
		
	--��coroutine����Ϣ�ҵ�����event�ĵȴ�������
	for i,Object in ipairs(tObjects) do
		table.insert(Info.m_tObjects,Object)
		assert( nil == Object.m_tCoroutine[co] )
		Object.m_tCoroutine[co]=Info
	end
		
	if uWaitTime ~= nil then
		--�ȴ�ʱ�䲻��nil,���Ҫע��һ��timedout��tick
		local WaitForTick = CWaitForTick:new(Info)
		WaitForTick.m_co = co
		Info.m_Tick=WaitForTick
		g_App:RegisterTick(WaitForTick,uWaitTime)
		WaitForTick:SetTickName("WaitForTick")
	end
	
	return coroutine.yield()
	
end
