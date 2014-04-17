engine_require "common/Misc/TypeCheck"

CEvent = class(CSyncObject)

function CEvent:Ctor(_bSignaled)
	CSyncObject.Ctor(self)
	if IsBoolean(_bSignaled) then
		self.m_bSignaled=_bSignaled
	else
		self.m_bSignaled=false
	end
end

function CEvent:IsReady()
	return self.m_bSignaled
end

--[[
	bEnable��	��Object����ΪEnable����Disabled
	���Object������ΪEnabled,�������еȴ���Object�ģ��������������ȴ�������coroutine.
--]]

function CEvent:Set( bEnabled )
	assert( IsBoolean( bEnabled ) )
	
	if bEnabled then
		
		--����ͬ������ΪEnabled
		if self.m_bSignaled then
			--ͬ���������ʹ���Enabled״̬����ôʲôҲ������
			return
		end
		
		--��ͬ��������Ϊenabled
		self.m_bSignaled = true
		
		self:_ResumeAllCo()
		
	else
		
		--����ͬ������ΪDisabled
		self.m_bSignaled=false
		
	end
end
