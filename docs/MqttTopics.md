# MQTT Topicks

## MQTTエンドポイント
```
// SSID
aterm-a55b9a-g
// または
aterm-a55b9a-a
// PASS
15208da0108bd

// MQTTの接続先のIP
192.168.179.7
// MQTTのポート
1883
```

## Topicks
* sub
    * マイコン側でsubしているトピック
    * /sub/bigakabeko
        * `{ value: true, time: 1000 }`
            * value: ブロアーのON/OFF, time: ON時間(ms)
    * /sub/target
        * /sub/target/akabeko
        * /sub/target/red
            * `{ value: true, time: 1000 }`
                * 倒れてから起き上がる時間(ms)
        * /sub/target/pink
            * `{ value: true, time: 1000 }`
                * 倒れてから起き上がる時間(ms)
        * /sub/target/white
            * `{ value: true, time: 1000 }`
                * 倒れてから起き上がる時間(ms)
        * /sub/target/yellow
            * `{ value: true, time: 1000 }`
                * 倒れてから起き上がる時間(ms)
    * /sub/gun
        * /sub/gun/predicted
            * 下記参照
* pub
    * マイコン側からsubされるトピック
    * /pub/gun
        * /pub/gun/image
            * `BASE64 Image Data`
                * 銃口のカメラの映像(1秒毎にpostされる)
        * /pub/gun/shot
            * `{ target: red }`
                * 対象

### /sub/gun/predicted のメッセージスキーマ
```json
{
    "items": [
        {
            "id": 8,
            "name": "赤べこ",
            "detected_area": [ 10, 5, 200, 250 ]
        }
    ]
}
```