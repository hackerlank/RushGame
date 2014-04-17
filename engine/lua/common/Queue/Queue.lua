CQueue = class()

function CQueue:clear()
	self._begin = nil
	self._end = nil
	self._count = 0
end

function CQueue:push(object)
	if nil ~= self._end then
		local node = { _value = object, _next=nil }
		self._end._next = node
		self._end = node
	else
		local node = { _value = object, _next=nil }
		self._begin = node
		self._end = node
	end
	self._count= self._count+1
end

-- �Ӷ���ͷ����һ��Ԫ�� 
function CQueue:pop()
	if nil ~= self._begin then
		local temp = self._begin._next
		self._begin = nil
		self._begin = temp
		self._count = self._count - 1
	end
	
	if self:empty() then
		self._end = nil
	end
end

-- ��ȡ������Ԫ�ص���Ŀ 
function CQueue:size()
	return self._count
end

-- �����Ƿ�Ϊ�� 
function CQueue:empty()
	return 0 == self._count
end

-- �õ�����ͷԪ�ص����� 
function CQueue:front()
	if not self:empty() then
		return self._begin._value
	else
		return nil
	end
end

-- �õ�����βԪ�ص����� 
function CQueue:back()
	if not self:empty() then
		return self._end._value
	else
		return nil
	end
end

-- �õ�һ����������


function CQueue:Ctor()
	self._count	= 0
end

-- sample
---------------------------------------------------
--local a = CQueue:new()
--local b = CQueue:new()
--
--a:push(a,"1111")
--a:push(a,"2222")
--a:push(a,"3333")


-----------------

--a:push("4444")
--
--b:push("5555")
--
--
----print(a:front())
--
--
--
--a:pop()
----print(a:front())
--a:pop()
--a:pop()
--a:pop()
--a:pop()
--
----print(b:back())
--
--b:pop()
--
----print(a:front())
--
----print(b:back())
--
--local count = a:size()
--
--local iter = a._begin
--while iter do
--	--print(iter._value)
--	iter = iter._next
--end
--
--count = a:size()
--
--if a:empty() then
--print "a is empty"
--end
--
--if b:empty() then
--print "b is empty"
--end --]]