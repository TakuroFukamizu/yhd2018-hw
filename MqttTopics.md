# MQTT Topicks

## Topicks
* sub
    * マイコン側でsubしているトピック
    * /sub/bigakabeko
        * `{ value: true }`
            * value: ブロアーのON/OFF: ON時間
    * /sub/target
        * /sub/target/akabeko
        * /sub/target/red
            * `{ value: 100 }`
                * 角度
        * /sub/target/pink
            * `{ value: 100 }`
                * 角度
        * /sub/target/white
            * `{ value: 100 }`
                * 角度
        * /sub/target/yellow
            * `{ value: 100 }`
                * 角度
* pub
    * マイコン側からsubされるトピック
    * /pub/gun
        * /pub/gun/shot
            * `{ target: red }`
                * 対象
