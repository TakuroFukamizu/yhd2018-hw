import paho.mqtt.client as mqtt
import json
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
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
            txtfile = file.read()
            jsonobj = json.loads(txtfile)
            if len(jsonobj["items"]) > 0:
                dataList.append(jsonobj)
        head -= 1

    return dataList

def trigger():
    dataList = load_buffer()
    print(dataList)

    for data in dataList:
        items = data["items"]
        for item in items:
            class_index = item["id"]
            if class_index not in [0, 1, 8]:
                continue
            # 名前を解決
            pubdata = {
                "target": names[class_index]
            }
            client.publish(topic, json.dumps(pubdata))

def buttonstate():
    try:
        while True:
            if GPIO.input(4) == False:
                print("OK")
                trigger()
    except KeyboardInterrupt:
        GPIO.cleanup()


if __name__ == '__main__':
    GPIO.setup(4, GPIO.IN,pull_up_down=GPIO.PUD_UP)
    client = mqtt.Client(protocol=mqtt.MQTTv311)
    client.on_connect = on_connect
    client.connect(host, port=port, keepalive=10)
    buttonstate()
    GPIO.cleanup()
