--[[
	Module����һ��module��ģ��lua��module����
	ModuleName����module������
	ModuleMeta��Module��metatable�����Ϊnil����metaΪglobal
	FuncEnv����module������environment��������function level��func����
--]]

local debug = debug
local package = package

local _ExportTbl = "_ExportTblAsClosure"

local _ModuleTbl = {}

function Module(ModuleName, ModuleMeta, FuncEnv)
	local ModuleTbl = {}
	
	assert(type(ModuleName) == "string")
	assert(not _ModuleTbl[ModuleName])
	_ModuleTbl[ModuleName] = ModuleTbl
	
	if not ModuleMeta then
		ModuleMeta = {
			__index = _G,
			__newindex = function(tbl, key, value)
				rawset(tbl, key, value)
			end		
		}
	else
		assert(type(ModuleMeta) == "table")
	end
	
	setmetatable(ModuleTbl, ModuleMeta)
	
	--��module����һ��������������еĵ��뺯��ֻ�ܴ������������
	rawset(ModuleTbl, _ExportTbl, {})
	
	--���û��FuncEnv����ô�������õ���Module�ĺ�����environment
	if type(FuncEnv) == "nil" then
		FuncEnv = 2
	end
	
	--����Ϊɶ����sefenv��ʹ��debug�⣬����Ϊlua�Ժ�İ汾�Ὣsetfenv��getfenv���ɵ���
	local info = debug.getinfo(FuncEnv, "f")
	
	debug.setfenv(info.func, ModuleTbl)
	
	return ModuleTbl
end


--[[
	�����������module������ʱ�򴫸�������module��Ϊ��metatable��
	һ������ֻ���ô�����module����һЩlua�ṩ��ȫ�ֺ����������ǲ��ܷ���ȫ�ֿռ��
	���������SandBox���͵ģ���ô���Ǵ�����module������ȫ�ֱ���
--]]
function GetModuleMt(SandBoxEnabled)
	local mt = {
		coroutine		= _G.coroutine,
		table			= _G.table,
		math			= _G.math,
		string			= _G.string,
		os				= _G.os,
		package			= _G.package,
		file			= _G.file,
		debug			= _G.debug,
		io				= _G.io,

		_VERSION		= _G.VERSION,
		assert 			= _G.assert,
		collectgarbage 	= _G.collectgarbage,
		dofile			= _G.dofile,
		error 			= _G.error,
		getfenv			= _G.getfenv,
		getmetatable 	= _G.getmetatable,
		ipairs 			= _G.ipairs,
		load			= _G.load,
		loadfile		= _G.loadfile,
		loadstring		= _G.loadstring,
		module			= _G.module,
		next 			= _G.next,
		pairs			= _G.pairs,
		pcall			= _G.pcall,
		print			= _G.print,
		rawequal		= _G.rawequal,
		rawget			= _G.rawget,
		rawset			= _G.rawset,
		require			= _G.require,
		select			= _G.select,
		setfenv			= _G.setfenv,
		setmetatable 	= _G.setmetatable,
		tonumber		= _G.tonumber,
		tostring		= _G.tostring,
		type			= _G.type,
		unpack			= _G.unpack,
		xpcall			= _G.xpcall,

		
		db				= 	_G.db,
		Export			=	_G.Export
	}
	
	for k, v in pairs(LoadPathTbl) do
		local ImportFunc = k .. "_import"
		mt[ImportFunc] = _G[ImportFunc]	

		local RequireFunc = k .. "_require"
		mt[RequireFunc] = _G[RequireFunc]	
	end
	
	mt.__index = function(tbl, key)
		return rawget(mt, key)
	end
	
	if SandBoxEnabled then
		mt.__newindex = function(tbl, key, value)
			if type(value) ~= "function" then
				error("���������ó��������������ȫ�ֱ���")
			end
			
			rawset(tbl, key, value)
		end	
	end
	
	return mt
end


AddCheckLeakFilterObj(Module)
AddCheckLeakFilterObj(GetModuleMt)





