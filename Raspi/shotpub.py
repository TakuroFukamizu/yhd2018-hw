import paho.mqtt.client as mqtt
import json
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(20, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

host = "192.168.179.7"
port = 1883
topic = "/pub/gun/shot"
# クラッピー追記お願いします
names = {
    8: "akabeko",
    0: "red",
    1: "yellow"
}

def on_connect(client, userdata, flags, respons_code):
    print('status {0}'.format(respons_code))

def load_buffer():
    head = 0
    with open("./predicted_ringbuffer/head.txt", "r") as file:
        head = int(file.read())
    count = 0
    with open("./predicted_ringbuffer/counter.txt", "r") as file:
        count = int(file.read())

    dataList = []
    for i in range(3):
        # カウント以上のファイルは読めない
        if (count < abs(head % 12)):
            break

        with open("./predicted_ringbuffer/buffer" + str(abs(head % 12)) + ".json", "r") as file:
            dataList.append(file.read())
        head -= 1

    return dataList

def trigger():
    dataList = load_buffer()

    for data in dataList:
        # 名前を解決
        pubdata = {
            "target": names[json.loads(data)["items"][0]["id"]]
        }
        client.publish(topic, json.dumps(pubdata))

def buttonstate():
    while True:
        if GPIO.input(24) == True:
            trigger()


if __name__ == '__main__':
    client = mqtt.Client(protocol=mqtt.MQTTv311)
    client.on_connect = on_connect
    client.connect(host, port=port, keepalive=10)
    buttonstate()