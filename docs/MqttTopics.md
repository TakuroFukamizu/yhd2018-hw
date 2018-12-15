# MQTT Topicks

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
* pub
    * マイコン側からsubされるトピック
    * /pub/gun
        * /pub/gun/shot
            * `{ target: red }`
                * 対象
