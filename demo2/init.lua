chipid = node.chipid()

wifi.setmode(wifi.STATIONAP)
wifi.ap.config({ ssid = 'nodemcu-'..tostring(chipid), auth = AUTH_OPEN })
