#### Feature
 - Map keyboard และส่ง Output ทาง Serial1 `0xFf,0xFF,0x__,0x__,0x__`
 - Switch Debouncing JoyStick และ keyboard 
 - กด JoyStick ด้าง 1 s จะส่ง Output อัตโนมัติ
 - ควบคุมเสียง Beep ผ่าน Serial1 
 - State Chraging โดยใช้เสียง Beep 1 ครั้งเมื่อชาร์ต และ Beep 2 ครั้งเมื่อ Un Charge
 - Sleep High Volt เมื่อไม่มีการรับ Input เกิน 1 นาที และ re charge Hight volt 3 วินาที
 - Check percent battery ผ่านทาง Serial1 `0x96`