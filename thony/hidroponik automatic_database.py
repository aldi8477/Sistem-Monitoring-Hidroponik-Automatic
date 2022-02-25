import paho.mqtt.client as mqtt
import mysql.connector
import json
	
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("hidroponik_polindra_data")
    
def on_message(client, userdata, msg):
    txt = msg.payload
    txt2 = txt.decode('UTF-8')
    mydb = mysql.connector.connect(host="localhost", user="root", password="", database="hidroponik")
#     print(txt2);
    insert_db(mydb, txt2)

def insert_db(mydb, txt2):
    mycursor = mydb.cursor()
    sql = "INSERT INTO data (sensor_tds) VALUES ('"+txt2+"')"
    mycursor.execute(sql)
    mydb.commit()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("broker.hivemq.com", 1883, 60)
client.loop_forever()

