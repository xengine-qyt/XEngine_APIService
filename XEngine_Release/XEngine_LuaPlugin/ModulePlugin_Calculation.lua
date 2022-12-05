function PluginCore_Init()
    return true
end

function PluginCore_UnInit()
end


function PluginCore_URLSqlit(str,reps)
    local resultStrList = {}
    string.gsub(str,'[^'..reps..']+',function (w)
        table.insert(resultStrList,w)
    end)
    return resultStrList
end


function PluginCore_Call(lpszStrUrl, nListCount, lpszMsgBuffer, nMsgLen)
    if nListCount < 3 then
        return false
    end

    local nValue = 0
    local HDRArray = PluginCore_URLSqlit(lpszStrUrl,'&')
    local HDRObjectValue1 = PluginCore_URLSqlit(HDRArray[1],'=')
    local HDRObjectValue2 = PluginCore_URLSqlit(HDRArray[2],'=')
    local HDRObjectType = PluginCore_URLSqlit(HDRArray[3],'=')

    if '0' == HDRObjectType[2] then
        nValue = HDRObjectValue1[2] + HDRObjectValue2[2]
    elseif '1' == HDRObjectType[2] then
        nValue = HDRObjectValue1[2] - HDRObjectValue2[2]
    elseif '2' == HDRObjectType[2] then
        nValue = HDRObjectValue1[2] * HDRObjectValue2[2]
    elseif '3' == HDRObjectType[2] then
        nValue = HDRObjectValue1[2] / HDRObjectValue2[2]
    end

    PInt_HTTPCode = 200
    PtszMsgBuffer = "{\"code\":0,\"value\":" .. nValue .. "}"
    PInt_MsgLen = #PtszMsgBuffer

    return true
end

-- PluginCore_Call1("param1=1&param2=2&param3=0",3,"hello",5)