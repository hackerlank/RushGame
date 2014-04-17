engine_require "common/Misc/TypeCheck"

CContextMgr = class(CContextMgr)

CContextMgr.m_nID = 0
CContextMgr.m_tblContext = {}

--���������ģ����ط����id
function CContextMgr:AddContext(context, OutTime)
	--print("---------->>>>>>>>>>>>>>>>CContextMgr:AddContext()")
	assert(IsTable(context))
	assert(IsNumber(OutTime))
	self.m_nID = self.m_nID + 1
	self.m_tblContext[self.m_nID] = context
	context.m_nID = self.m_nID
	context.m_ContextMgr = self
	context.m_App:RegisterTick(context,OutTime)
	context:SetTickName("ContextTick")
	return self.m_nID
end

--ȡ��ĳ�������ģ���ɾ��֮
function CContextMgr:DelContext(id)
	--print("---------->>>>>>>>>>>>>>>>CContextMgr:DelContext()")
	--print(id)
	assert(IsNumber(id))
	local context = self.m_tblContext[id]
	if(self.m_tblContext[id] ~= nil) then
		self.m_tblContext[id].m_App:UnRegisterTick(self.m_tblContext[id])
		self.m_tblContext[id] = nil
	end
	return context
end