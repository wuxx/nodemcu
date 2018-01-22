--chipid = node.chipid()
--wifi.setmode(wifi.STATIONAP)
--wifi.ap.config({ ssid = 'nodemcu-'..tostring(chipid), auth = AUTH_OPEN })
--[[ wifi --]]

function wifi_init()
    local c = 0
    local ssid = "PandoraBox_0D0D47"
    local pwd  = "wujianhua"
    
    local ret  = -1
    
    wifi.setmode(wifi.STATION)
    station_cfg={}
    station_cfg.ssid=ssid
    station_cfg.pwd=pwd
    wifi.sta.config(station_cfg)
    wifi.sta.connect()
    tmr.alarm(1, 1000, 1, function()
        if wifi.sta.getip() == nil then
            print("IP unavaiable, waiting...")
            c = c + 1
            if (c == 10) then
                print("wait timeout, exit.")
                tmr.stop(1)
                ret = 1
            end
        else
            tmr.stop(1)
            print("ESP8266 mode is: " .. wifi.getmode())
            print("The module MAC address is: " .. wifi.ap.getmac())
            print("config done, IP is "..wifi.sta.getip())
            ret = 0
        end
    end)
    
    print("ret is: " .. ret)
end

wifi_init()

