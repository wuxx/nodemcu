-------------
-- define
-------------
IO_LED = 1
IO_LED_AP = 2
IO_BTN_CFG = 3
IO_BLINK = 0

TMR_WIFI = 4
TMR_BLINK = 5
TMR_BTN = 6

gpio.mode(IO_LED, gpio.OUTPUT)
gpio.mode(IO_LED_AP, gpio.OUTPUT)
gpio.mode(IO_BTN_CFG, gpio.INT)
gpio.mode(IO_BLINK, gpio.OUTPUT)

-------------
-- button
-------------
function onBtnEvent()
	gpio.trig(IO_BTN_CFG)
	tmr.alarm(TMR_BTN, 500, tmr.ALARM_SINGLE, function()
		gpio.trig(IO_BTN_CFG, 'up', onBtnEvent)
	end)

	switchCfg()
end
gpio.trig(IO_BTN_CFG, 'up', onBtnEvent)

gpio.write(IO_LED_AP, gpio.LOW)

function switchCfg()
	if wifi.getmode() == wifi.STATION then
        print("set STATIONAP")
		wifi.setmode(wifi.STATIONAP)
		httpServer:listen(80)
		gpio.write(IO_LED_AP, gpio.HIGH)
	else
        print("set STATION")
		wifi.setmode(wifi.STATION)
		httpServer:close()
		gpio.write(IO_LED_AP, gpio.LOW)
	end
end

-------------
-- blink
-------------
blink = nil
tmr.register(TMR_BLINK, 100, tmr.ALARM_AUTO, function()
	gpio.write(IO_BLINK, blink.i % 2)
	tmr.interval(TMR_BLINK, blink[blink.i + 1])
	blink.i = (blink.i + 1) % #blink
end)

function blinking(param)
	if type(param) == 'table' then
		blink = param
		blink.i = 0
		tmr.interval(TMR_BLINK, 1)
		running, _ = tmr.state(TMR_BLINK)
		if running ~= true then
			tmr.start(TMR_BLINK)
		end
	else
		tmr.stop(TMR_BLINK)
		gpio.write(IO_BLINK, param or gpio.LOW)
	end
end


status = nil

wifi.eventmon.register(wifi.STA_WRONGPWD, function()
	--blinking({100, 100 , 100, 500})
	status = 'STA_WRONGPWD'
	print(status)
end)

wifi.eventmon.register(wifi.STA_APNOTFOUND, function()
	--blinking({2000, 2000})
	status = 'STA_APNOTFOUND'
    print(status)
    print(wifi.sta.getip())
    --print(status, wifi.sta.getip())
end)

wifi.eventmon.register(wifi.STA_CONNECTING, function(previous_State)
	--blinking({300, 300})
	status = 'STA_CONNECTING'
	print(status)
end)

wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function(T)
	--blinking()
	status = 'STA_GOTIP'
	print(status)
    print(status, wifi.sta.getip())
    print("\n\tSTA - GOT IP".."\n\tStation IP: "..T.IP)
end)

--wifi.sta.eventMonStart(1000)

-------------
-- wifi
-------------
print('Setting up WIFI...')
--wifi.sta.config('MY_SSID', 'MY_PASSWORD')
station_cfg={}
station_cfg.ssid="PandoraBox_0D0D47"
station_cfg.pwd="wujianhuax"
wifi.sta.config(station_cfg)

chipid = node.chipid()
wifi.ap.config({ ssid = 'nodemcu-'..tostring(chipid), auth = AUTH_OPEN })
wifi.setmode(wifi.STATIONAP)
--wifi.setmode(wifi.STATION)
wifi.sta.autoconnect(1)
--wifi.sta.connect()


-------------
-- http
-------------
dofile('httpServer.lua')
httpServer:listen(80)

httpServer:use('/config', function(req, res)
	if req.query.ssid ~= nil and req.query.pwd ~= nil then
        station_cfg1 = {}
        station_cfg1.ssid=req.query.ssid
        station_cfg1.pwd=req.query.pwd
        --wifi.setmode(wifi.STATION)
        wifi.sta.config(station_cfg1)
		--wifi.sta.connect()
        print("ssid-"..station_cfg1.ssid)
        print("pwd-"..station_cfg1.pwd)
       
		status = 'STA_CONNECTING'
		tmr.alarm(TMR_WIFI, 1000, tmr.ALARM_AUTO, function()
			if status ~= 'STA_CONNECTING' then
				res:type('application/json')
				res:send('{"status":"' .. status .. '"}')
				tmr.stop(TMR_WIFI)
			end
		end)
	end
end)

httpServer:use('/scanap', function(req, res)
	wifi.sta.getap(function(table)
		local aptable = {}
		for ssid,v in pairs(table) do
			local authmode, rssi, bssid, channel = string.match(v, "([^,]+),([^,]+),([^,]+),([^,]+)")
			aptable[ssid] = {
				authmode = authmode,
				rssi = rssi,
				bssid = bssid,
				channel = channel
			}
		end
		res:type('application/json')
		res:send(sjson.encode(aptable))
	end)
end)
