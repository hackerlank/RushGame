engine_require "common/Loader/export_loader"

local LoadedCfgTbl = LoadedCfgTbl
local GetLoadPath = GetLoadPath

local _eTFFT_String = ETableFileFieldType.eTFFT_String
local _eTFFT_Number = ETableFileFieldType.eTFFT_Number
local _GetHeight= CMultiLangTxtTableFile.GetHeight
local _GetWidth = CMultiLangTxtTableFile.GetWidth
local _GetType = CMultiLangTxtTableFile.GetType
local _GetTypeByName = CMultiLangTxtTableFile.GetTypeByName
local _GetString = CMultiLangTxtTableFile.GetString
local _GetStringByName = CMultiLangTxtTableFile.GetStringByName
local _GetNumber= CMultiLangTxtTableFile.GetNumber
local _GetNumberByName = CMultiLangTxtTableFile.GetNumberByName
local _GetValue = CMultiLangTxtTableFile.GetValue
local _GetValueByName = CMultiLangTxtTableFile.GetValueByName
local _Load = CMultiLangTxtTableFile.Load

local TxtData = CTableFileValue:new()

function CMultiLangTxtTableFile.Clear()
	TxtData = nil
end

function CMultiLangTxtTableFile:GetData(RowIndex, ColIndex)
	return _GetValue(self, TxtData, RowIndex, ColIndex)
end

function CMultiLangTxtTableFile:GetDataByName(RowIndex, ColName)
	return _GetValueByName(self, TxtData, RowIndex, ColName)
end

local _GetData= CMultiLangTxtTableFile.GetData
local _GetDataByName = CMultiLangTxtTableFile.GetDataByName

CMultiLangTxtTableFile.__call = function(tbl, RowKey, ColName)
				if RowKey == nil and ColName == nil then
					return nil
				elseif RowKey ~= nil and ColName ~= nil then
					--RowName��ColName����Ϊ�գ����Ƿ�����Ӧ������
					assert(type(ColName) == "string")
					
					local Row = tbl.RowKey2Index[RowKey]
					if not Row then
						return nil
					end
					
					return _GetDataByName(tbl, Row, ColName)
					
				elseif RowKey ~= nil and ColName == nil then
					local Row = tbl.RowKey2Index[RowKey]
					if not Row then
						return nil
					end
					
					--RowName��Ϊ�գ����Ƿ���һ����������������ܹ�ֱ��ͨ�������colname�õ�����
					
					return function(ColIndexName)
						assert(type(ColIndexName) == "string")
						
						return _GetDataByName(tbl, Row, ColIndexName)
					end
				else
					assert(type(ColName) == "string")
					return function(RowIndexKey)
						local Row = tbl.RowKey2Index[RowKey]
						if not Row then
							return nil
						end
												
						return _GetDataByName(TableFile, Row, ColName)
					end
				end								
			end
			

function CMultiLangTxtTableFile:GetKeys()
	return self.RowKeys
end
			
CMultiLangTxtTableFile.__newindex = function(tbl, key, value)
	error("can't set value in readonly table")
end

local function DummyFunc()
end

--�µĵ��б����ʹ�ú���������õ��б�ͨ����������
function LoadLanSingle(Language, AliasPath, ModuleName, ...)
	if Language == "" and LoadedCfgTbl[ModuleName] then
		return LoadedCfgTbl[ModuleName]
	end
	
	local argc = {...}
	local argn = select("#", ...)

	local function _LoadSingle()
		local TableFile = CMultiLangTxtTableFile:new()
		local ret = TableFile:Load(AliasPath, unpack(argc, 1, argn))
		if not ret then
			local msg = ModuleName .. " ������������ļ�"
			error(msg)
		end
	
		local RowKey2Index = {}
		local RowKeys = {}
		
		local Height = _GetHeight(TableFile)
		
		--Ϊÿһ�е������������ö�Ӧ���кţ����ǵĵ��б��row��ͨ����һ�е����������е�
		for i = 1, Height - 1 do
			local IndexKey = _GetData(TableFile, i, 0)
			assert(not RowKey2Index[IndexKey],"���ñ�"..ModuleName.."����������"..IndexKey.."���ظ�")
			RowKey2Index[IndexKey] = i	
			table.insert(RowKeys, IndexKey)
		end
		
			
		rawset(TableFile, "RowKey2Index", RowKey2Index)
		rawset(TableFile, "RowKeys", RowKeys)	
	
		local function GetHeight()
			return Height - 1
		end
		
		rawset(TableFile, "GetHeight", GetHeight)
		
		if Language == "" then
			_G[ModuleName] = TableFile
		else
			if _G["Lan_" .. Language] == nil then
				_G["Lan_" .. Language] = {}
			end
			_G["Lan_" .. Language][ModuleName] = TableFile
		end
		LoadedCfgTbl[ModuleName] = TableFile
		
		AddCheckLeakFilterObj(TableFile)
	
		return TableFile		
	end

	if ExportClassEnabled() then
		return ExportCfgLoad(Language, ModuleName, _LoadSingle)
	else
		return _LoadSingle()
	end
end


