import paho.mqtt.client as mqtt
import json
from .oled.PrintDetects import print_detects

host = "192.168.179.7"
port = 1883
topic = "/sub/gun/predicted"

def on_connect(client, userdata, flags, respons_code):
    print('status {0}'.format(respons_code))
    client.subscribe(topic)

counter = 0
def on_message(client, userdata, msg):
    global counter
    message = msg.payload.decode(encoding='utf-8')
    json_data = json.loads(message)
    items = [ (items["id"], items["name"], items["detected_area"]) for item in json_data["item"] ]
    print_detects(items)

    with open("./predicted_ringbuffer/buffer" + str(counter % 12) + ".json", "w") as file:
        file.write(json.dumps(json_data))
    with open("./predicted_ringbuffer/head.txt", "w") as file:
        file.write(str(counter % 12))
    with open("./predicted_ringbuffer/counter.txt", "w") as file:
        file.write(str(counter))

    counter = counter + 1

if __name__ == '__main__':
    client = mqtt.Client(protocol=mqtt.MQTTv311)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(host, port=port, keepalive=10)
    client.loop_forever()
