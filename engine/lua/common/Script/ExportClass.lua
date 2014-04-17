--[[
	dynamic export class information
	
	ClassName, FileName, BaseClassName1, BaseClassName2, ...
	
	
	local tbl = {
		{"ClassName", "framework/main_frame/ClassName", "BaseName1", "BaseName2"},
		......
	}
--]]

--[[
	����lua��export class��������
	��һ����c++ע���class������class��������lua�����ж�Ӧ�������ˣ�
	������Ϊclass new�����Ķ��������tableҲ������userdata���������ǵ�ͨ��
	�滻class��new�Լ�class��metatable������export����
	
	�ڶ�����luaʹ��class����ע���class����������ʹ��һ��dummy class����ʾ��
	��Ϊ���Ķ�����������Ҫrequire�Ķ����ļ�����ģ�������require���ļ�֮��
	���ǻὫdummy�滻��ʵ�ʵ�class
--]]

local eSCT_Isolated = EScriptClassType.eSCT_Isolated
local eSCT_Static = EScriptClassType.eSCT_Static
local eSCT_Dynamic = EScriptClassType.eSCT_Dynamic
local eSCT_Share = EScriptClassType.eSCT_Share

local str_len = string.len

local ExportClassTbl = {}

local function IsCppClass(Class)
	if rawget(Class, "__info") then
		return true
	end
	
	return false
end


local function _ExportClass(Class)
	local Info = rawget(Class, "__ExportInfo")
	
	local ClassName = rawget(Class, "__ClassName")
	
	if not Info then
	    if ClassName then
	        return _G[ClassName]
	    else
	        return Class
	    end
	end
			
	--�����export info��Ϣ��������Ҫexport class��
	rawset(Class, "__ExportInfo", nil)
	
	local bCppClass = IsCppClass(Class)
	if bCppClass then
		local ClassNew = rawget(Class, "__orign_new")
		rawset(Class, "new", ClassNew)
		rawset(Class, "__orign_new", nil)

		local ClassIndex = rawget(Class, "__orign_index")
		rawset(Class, "__index", ClassIndex)
		rawset(Class, "__orign_index", nil)
		
		local ClassCall = rawget(Class, "__orign_call")
		rawset(Class, "__call", ClassCall)
		rawset(Class, "__orign_call", nil)
	
		local ClassMt = rawget(Class, "__orign_mt")
		setmetatable(Class, ClassMt)
		rawset(Class, "__orign_mt", nil)			

		RequireFile(Info[1], Info[2])
		
		ClassMt.__newindex = function(tbl, key, value)
			error("��������ֻ���������������")
		end

		return Class
	else
		--�����lua�࣬��ô�������Ƚ�dummy class�����ȫ�ֿռ�
		
		_G[ClassName] = nil

		--�����ļ�
		RequireFile(Info[1], Info[2])			
		
		--�õ�ʵ�ʵ�lua��
		local NewClass = _G[ClassName]
		
		NewClass["__ClassName"] = ClassName
		
		assert(NewClass)
		
		--�������֮�����ǵ������ֻ������
		local NewClassMt = getmetatable(NewClass)
		
		NewClassMt.__newindex = function(tbl, key, value)
			error("��������ֻ���������������")
		end
	
		
		return NewClass
	end
end

local ExportFileName = {}

local function _DynamicExportClass(Class)
	local NewClass = _ExportClass(Class)
	
	local BaseClassList = rawget(NewClass, "__base_list")
	if BaseClassList then
		local n = #BaseClassList
		for i = 1, n do
			local BaseClass = BaseClassList[i]
			BaseClassList[i] = _ExportClass(BaseClass)
		end
	end
	
	return NewClass
end

local function _RegExportClass(ClassName, FileName)
	assert(type(ClassName) == "string")
	assert(type(FileName) == "string")
	
	if ExportFileName[FileName] then
		error(FileName .. "�Ѿ���ע���ˣ�һ���ļ�ֻ����ע��һ����")
	end
	
	ExportFileName[FileName] = true	
		
	if ExportClassTbl[ClassName] then
		error(ClassName .. "�Ѿ����óɵ����࣬�������ٴ�����")
	end
	
	ExportClassTbl[ClassName] = true
end

local function _CheckCppClass(ClassName)
	--����������c++�࣬��ô����������ʱ���Ѿ�������ȫ�ֱ�������
	if ClassObj then
		if not rawget(ClassObj, "__info") then
			error(ClassName .. "������ȫ�ֿռ䲢�Ҳ���ע���C++��")		
		end
		
		local ClassType = rawget(ClassObj, "__ClassType")
		if ClassType == eSCT_Share then
			error(ClassName .. "��share�࣬����������Ϊ������")
		end
	end
end

local function _SetCppExportClass(CppClass)
	CppClass.__orign_new = CppClass.new
	
	CppClass.__orign_index = CppClass.__index
	CppClass.__orign_call = CppClass.__call
	
	CppClass.__index = function(tbl, key)
		_DynamicExportClass(CppClass)
		
		local CtorFunc = CppClass.Ctor
		if CtorFunc then
			CtorFunc(tbl)
		end
				
		local IndexFunc = rawget(CppClass, "__index")
		return IndexFunc(tbl, key)	
	end
	
	CppClass.__call = function(tbl, ...)
		_DynamicExportClass(CppClass)
		
		local CtorFunc = CppClass.Ctor
		if CtorFunc then
			CtorFunc(tbl)
		end
		
		local CallFunc = rawget(CppClass, "__call")
		return CallFunc(tbl, ...)	
	end

	
	--��Ϊ���ó���export class���������ǵ�new����ҲҪ��д��
	function CppClass:new(...)
		local n = select("#", ...)
		--���new��ʱ���в�������ô���Ǿ�Ҫֱ�������ļ���������Ctor
		if n > 0 then
			_DynamicExportClass(CppClass)
			local NewInstance = CppClass:new(...)
			return NewInstance
		else
			local NewInstance = CppClass.ctor( CppClass )

			--[[
			local function ExportFunc(tbl, CppClass)
				_DynamicExportClass(CppClass)
					
				ChangeMetaTable(tbl, CppClass)
					
				local CtorFunc = CppClass.Ctor
				if CtorFunc then
					CtorFunc(tbl)
				end
			end

			local DummyClass = {
				__index = function(tbl, key)
					ExportFunc(tbl, CppClass)
					
					local IndexFunc = rawget(CppClass, "__index")
					return IndexFunc(tbl, key)
				end,
				
				__call = function(tbl, ...)
					ExportFunc(tbl, CppClass)
					
					local CallFunc = rawget(CppClass, "__call")
					return CallFunc(tbl, ...)
				end,
				
				__newindex = function(tbl, key, value)
					CppClass.__newindex(tbl, key, value)
				end,
			}		
			
			ChangeMetaTable(NewInstance, DummyClass)
			--]]
			
			return NewInstance
		end
		
	end
		
	--���潫export class��һЩmeta������д����������һ�ε��õ�ʱ����������
	
	local ClassMt = getmetatable(CppClass)
	assert(ClassMt)
	
	CppClass.__orign_mt = ClassMt
	
	local NewMt = {
		__index = function(Class, key)
			_DynamicExportClass(Class)
			
			--��ʱ��class��mt�Ѿ��ı��ˣ�������Ҫʹ���µ�mt��index������
			local ClassMt = getmetatable(Class)
			local IndexFunc = rawget(ClassMt, "__index")
						
			return IndexFunc(Class, key)
		end,
		
		__newindex = function(Class, key, value)
			rawset(Class, key, value)
		end,
	}
	
	setmetatable(CppClass, NewMt)
	
end

local function _SetLuaExportClass(LuaClass)
	local ClassMt = 
	{
		__index = function(Class, key)
			local NewClass = _DynamicExportClass(Class)
			
			local NewClassMt = getmetatable(NewClass)
			local IndexFunc = rawget(NewClassMt, "__index")
			
			return IndexFunc(NewClass, key)
		end,
	
		--����lua��class��˵����Ϊ��û����֮ǰ����һ��dummy class���������ǽ�ֹ__newindex
		__newindex = function(Class, key, value)
			error("��������ֻ���������������")
		end,
	}	
		
	function LuaClass:new(...)
		local n = select("#", ...)
		if n > 0 then
			local Class = _DynamicExportClass(LuaClass)	
			local Inst = Class:new(...)
			return Inst
		else
			local function ExportFunc(tbl, LuaClass)
				local Class = _DynamicExportClass(LuaClass)
					
				local init_obj = rawget(Class, "init_obj")
				
				if init_obj then
					--�����c++����������Ϣ���õ�obj����
					init_obj(Class, tbl)
				else
					setmetatable(tbl, Class)
				end
									
				--�����Ctor����һ��index��ʱ����뱣֤Ctor����
				local CtorFunc = Class.Ctor
				if CtorFunc then
					CtorFunc(tbl)
				end
				
				return Class, init_obj
			end
			
			local DummyClass = 
			{
				__index = function(tbl, key)
					local Class, init_obj = ExportFunc(tbl, LuaClass)
					
					if init_obj then
						--��Ϊ�����init_obj����ô�ͱ�������һ����һ��cpp class
						--��ʱ�����ǻὫһЩ��Ϣװ��������棬���������ж�key
						--�ǲ�����װ����ֶε�
						local value = rawget(tbl, key)
						if value then
							return value
						end
					end
					
					local IndexFunc = rawget(Class, "__index")
					return IndexFunc(tbl, key)
				end,
				
				__call = function(tbl, ...)
					local Class, init_obj = ExportFunc(tbl, LuaClass)
					
					if init_obj then
						local value = rawget(tbl, key)
						if value then
							return value
						end
					end
					
					local CallFunc = rawget(Class, "__call")
					return CallFunc(tbl, key)
				end,
			
				
				__newindex = function(tbl, key, value)
					rawset(tbl, key, value)
				end,
			
			}
			
			local Inst = {}
			setmetatable(Inst, DummyClass)
			return Inst
		end
	end

	setmetatable(LuaClass, ClassMt)
end

local function _CreateExportClass(AliasPath, ClassName, FileName)
	local ClassObj = _G[ClassName]
	
	_CheckCppClass(ClassObj)
	
	--�������c++ע���࣬��ô���ǿ��Դ���һ��lua����
	if not ClassObj then
		ClassObj = {}
		ClassObj["__ClassName"] = ClassName
		
		--����Ϊexport class info
		ClassObj["__ExportInfo"] = { AliasPath, FileName }
		
		_G[ClassName] = ClassObj
		
		_SetLuaExportClass(ClassObj)	
	else
		--����Ϊexport class info
		ClassObj["__ExportInfo"] = { AliasPath, FileName }
	
		_SetCppExportClass(ClassObj)
	end


end

local ls_ExportClassEnabled = false

function EnableExportClass(Enabled)
	ls_ExportClassEnabled = Enabled
end

function ExportClassEnabled()
	return ls_ExportClassEnabled
end

local RequireFuncTbl = {}
local ExportInfoTbl = {}

function LoadAllExportClass()
	if ls_ExportClassEnabled then
		ExportInfoTbl = {}
		return
	end

	for k, v in pairs(ExportInfoTbl) do
		_DynamicExportClass(v)
	end
end

AddCheckLeakFilterObj(LoadAllExportClass)

function SetExportClass(AliasPath, ClassTbl)
	assert(type(AliasPath) == "string")

	local FuncName = AliasPath .. "_require"
	local RequireFunc = _G[FuncName]
	assert(RequireFunc)
	
	if not RequireFuncTbl[FuncName] then
		_G[FuncName] = function(FileName)
			if ExportFileName[FileName] then
				error(FileName .. "���ó��˵����࣬������ֱ��require�䶨���ļ�")
			end
			
			return RequireFile(AliasPath, FileName)
		end
		
		RequireFuncTbl[FuncName] = true		
	end
	

	local n = #ClassTbl
	for i = 1, n do
		local ClassInfo = ClassTbl[i]
		local ClassName = ClassInfo[1]
		local FileName = ClassInfo[2]
				
		_RegExportClass(ClassName, FileName)
		
		_CreateExportClass(AliasPath, ClassName, FileName)		
		
		table.insert(ExportInfoTbl, _G[ClassName])
	end		


end

AddCheckLeakFilterObj(SetExportClass)
