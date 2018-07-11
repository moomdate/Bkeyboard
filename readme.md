#### Feature
 - Map keyboard และส่ง Output ทาง Serial1 `0xFf,0xFF,0x__,0x__,0x__`
 - Switch Debouncing JoyStick และ keyboard 
 - กด JoyStick ด้าง 1 s จะส่ง Output อัตโนมัติ
 - ควบคุมเสียง Beep ผ่าน Serial1 
 - State Chraging โดยใช้เสียง Beep 1 ครั้งเมื่อชาร์ต และ Beep 2 ครั้งเมื่อ Un Charge
 - Sleep High Volt เมื่อไม่มีการรับ Input เกิน 1 นาที และ re charge Hight volt 3 วินาที
 - Check percent battery ผ่านทาง Serial1 `0x96`

#### Serial Command 
    dec:155	-> Output:beep beep
    dec:154	-> Output:beep
    dec:153 -> Output:beeeep beeep (เสียงต่ำ)
    dec:152 -> Output:beep bep
    dec:151 -> Output:beep beep beeep
    dec:150 -> volt status 
            Output:percent battery (0-100)%
    dec:149 -> unmute
            Output:beep
    dec:148 -> mute
            Output:beep beep
