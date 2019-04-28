#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>

const char* ssid = "hs";
const char* password = "fzp5252621";
//SoftwareSerial mySerial(3,4); // RX, TX 

//最大连接数
#define MAX_SRV_CLIENTS 2  
//创建server 端口号是6666
WiFiServer server(6666);
//管理clients
WiFiClient serverClients[MAX_SRV_CLIENTS];



void setup() {
  
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);       //设置为站点模式
  WiFi.begin(ssid, password);
//  Serial.print("Connecting to WiFi ");
//
//  while (WiFi.status() != WL_CONNECTED) 
//  {
//       delay(500);
//       Serial .print(".");
//  }                               //判断是否连接上wifi
//  Serial.println(" ");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());   //WiFi.localIP()返回8266获得的ip地址
  server.begin();
  server.setNoDelay(true);  

  ESP.wdtEnable(5000);       //看门狗的触发间隔
  

  
}

void loop() 
{


  uint8_t index;
  if (server.hasClient())
  {
        for (index = 0; index < MAX_SRV_CLIENTS; index++)
        {
            if (!serverClients[index] || !serverClients[index].connected())
            {
                if (serverClients[index]) serverClients[index].stop();         //未联接,就释放
                serverClients[index] = server.available();//分配新的
                continue;
            }
        }
        //8266tcpserver只能接入五个  超出的需要释放
        WiFiClient serverClient = server.available();
        if (serverClient){
          serverClient.stop();
        }
  }

  for (index = 0; index < MAX_SRV_CLIENTS; index++)
  {
        if (serverClients[index] && serverClients[index].connected())
        {
            //处理客户端发过来的数据
            if (serverClients[index].available())
            {
                while (serverClients[index].available()) 
                {
                    //接收数据
                    Serial.write(serverClients[index].read());

                    
                }
                     
            }
            /*if(Serial.available()>0)
            {
                    //发送数据
                    
                   serverClients[index].write(Serial.read()); 
                   
          
            }*/
            
            if(Serial.available()>0)
            {
              
                   
                    
                      
                    size_t len = Serial.available(); 
                    uint8_t buff[len];
                    Serial.readBytes(buff, len);
                    serverClients[index].write(buff,len);
                    memset(buff,0,len);
                    
                    delay(1000);
                 
            }
        }
        
   }
   

   //喂狗
   ESP.wdtFeed();


}
