import paho.mqtt.client as mqttClient
import time
 
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker")
 
        global Connected               
        Connected = True               
 
    else:
        print("Connection failed")

def on_log(client, userdata, level, buf):
    print("log : ", buf)

Connected = False   
 
broker_address= 'localhost'
port = 1883
#user = "yourUser"
#password = "yourPassword"
 
client = mqttClient.Client('hello')               
#client.username_pw_set(user, password=password)    
client.on_connect= on_connect                      

# 브로커 연결
#client.connect(broker_address, port=port)          
client.connect("broker.mqttdashboard.com", 1883)
 
#루프 시작
client.loop_start()       
 
# 연결 기다리기
while Connected != True:    
    time.sleep(0.1)
 
try:
    while True:
        value = input('Enter the message: ')
        client.publish('hello', value)
#        client.on_log = on_log
 
except KeyboardInterrupt:
    client.disconnect()
    client.loop_stop()