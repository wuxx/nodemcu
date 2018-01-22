wifi.setmode(wifi.STATION)
station_cfg={}
station_cfg.ssid="PandoraBox_0D0D47"
station_cfg.pwd="wujianhua"
wifi.sta.config(station_cfg)
wifi.sta.connect()
print(wifi.sta.getip())

srv=net.createServer(net.TCP, 30)

print("server up")
srv:listen(80, function(c)   

    c:on("receive",function(c, req) 
        if req:find("favicon") then
            c:close();
            return
        end
        print("req:["..req.."]")
        if req:find("JDQ") then
            local setInfoStart = string.find(req,"JDQ",1)
            local setInfoEnd = string.find(req,"HTTP/1.1",1)
            local setInfo = string.sub(req,setInfoStart + 4,setInfoEnd - 2)
            print("INFO:["..setInfo.."]")
            local map = {0, 1};
            for k,v in pairs({'Ja','Jb'}) do
                local start = string.find(setInfo,v,1);
                print(v.." : "..string.sub(setInfo, start + 3, start + 3));
                start = string.sub(setInfo, start + 3,start + 3);
                if(start == "0") then
                    gpio.write(map[k], gpio.LOW);
                else
                    gpio.write(map[k], gpio.HIGH);
                end
            end
            return
        else
            file.open("index.html")
            local html = ""
            local temp = ""
            temp = file.readline()
            while(temp ~= nil) do
                html = html..temp
                temp = file.readline()
            end
            c:send(html)
            file.close()
            return
        end
    end)
end)
