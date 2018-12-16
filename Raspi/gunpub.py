# /pub/gun/shotで送信するところ
import paho.mqtt.client as mqtt
import json
import random
import time


host = "192.168.179.7"
port = 1883
topic = "/pub/gun/shot"

client = mqtt.Client(protocol=mqtt.MQTTv311)

client.connect(host, port=port, keepalive=10)

pubdict = {"topic": ""}

# 最初の方だったのでランダムでjsonを生成してました
def choice():
    return random.choice(["red", "clap"])

# 疎通確認のためにfor文でランダムにjsonを送っていました。
# リングバッファの実装をお願いしたいです。
for i in range(10):
    print(i)
    pubdict["topic"] = choice()
    jsn_str = json.dumps(pubdict)
    client.publish(topic, jsn_str)
    time.sleep(0.5)

client.disconnect()
