# MQTT Topicks

## Topicks
* sub
    * マイコン側でsubしているトピック
    * /sub/bigakabeko
        * `true 500`
            * ブロアーのON/OFF: ON時間
    * /sub/target
        * /sub/target/akabeko
        * /sub/target/red
            * `100`
                * 角度
        * /sub/target/pink
            * `100`
                * 角度
        * /sub/target/white
            * `100`
                * 角度
        * /sub/target/yellow
            * `100`
                * 角度
* pub
    * マイコン側からsubされるトピック
    * /pub/gun
        * /pub/gun/shot
            * `red`
                * 対象
