--[[
	CResumeTick
	Ϊ�˷�ֹ��ջ���ң�������obj׼����resume֮ǰ������һ��tick��ͨ����tick������resume
--]]

CWaitForCoInfo = class()

local CResumeTick = class(CTick)

function CResumeTick:Ctor(co, tResultObj)
	assert(not IsNil(co))
	assert(not IsNil(tResultObj))
	
	self.m_co = co
	self.m_tResultObj = tResultObj
end

function CResumeTick:OnTick()
	g_App:UnRegisterTick(self)
	local co=self.m_co
	local tResultObj = self.m_tResultObj
	
	for k, v in ipairs (tResultObj) do
		v.m_tCoroutine[co] = nil
	end
	
	coroutine.resume( co, tResultObj )
end

function CWaitForCoInfo:Ctor(bWaitAll)
	self.m_co = coroutine.running()
	self.m_tObjects = {}
	self.m_bWaitAll=bWaitAll
end


function CWaitForCoInfo:ResumeIfReady( Obj, Co )
	
	local tResultObj={}

	if self.m_bWaitAll then
		
		--��Ҫ�ȴ�����event,��˱������coroutine��ǰ���ڵȴ�������event,ȷ�����Ƕ���enabled״̬
		for i,OtherObj in ipairs(self.m_tObjects) do
					
			if not OtherObj:IsReady() then
				--������һ��Not Readyͬ������˵����coroutine���ڵȴ�����δ�����Object,��˲���Ҫ���Ѹ�coroutine
				return 
			end
			
			--OtherObj�Ѿ����������������
			table.insert( tResultObj, OtherObj )
			
		end
		
	else
		--�ȴ�����һ��Object������Ϊenabled,��Ϊ��ǰObject���ڱ�����Ϊenabled,���Ը�coroutine�϶�Ҫ������
		--���ﲻ��Ҫ�ٱ������е�Object,���˵�ǰObject֮�⣬����Object�϶���Not Ready״̬��	
		table.insert( tResultObj, Obj )
		
	end
	
	if Obj._Take then
		Obj:_Take()
	end
	
	if self.m_Tick then
		g_App:UnRegisterTick(self.m_Tick)
	end
	
	local ResumeTick = CResumeTick:new(Co, tResultObj)
	g_App:RegisterTick(ResumeTick, 0)
	ResumeTick:SetTickName("ResumeTick")
	
	--coroutine.resume(Co, tResultObj)
	
end