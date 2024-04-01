
<!-- For new line: \ -->
<!-- Picture: <img src="Photos/" width="300"/>  -->
<!-- New Line: "  <br>"  -->

# Про проект DRM Watch v3
Наручний годинник на базі ESP32-S2 та SHARP Memory LCD.
А все починалось з того, що мені сподобався дисплей...


<p align="center"><img src="Photos/DRM_zb2.png" width="100"/></p>
<p align="center">
<img src="Photos/photo_2024-02-04_23-56-24.jpg" width="200"/>
<img src="Photos/photo_2024-02-04_16-33-34.jpg" width="200"/>
<img src="Photos/photo_2024-02-04_16-33-34 (2).jpg" width="200"/>
</p>


## Особливості годинника
- Стабільно працює і підходить для щоденного користування.
- Дисплей завжди показує час;
- Акумулятора вистачає на понад 20днів;
- Зручні меню, звідки можна отримати доступ до всіх функцій;
- Наявність Wi-Fi, можна зберегти до 8 мереж в пам'яті;
- Можливість синхронізації часу по Wi-Fi;
- Багато мелодій. Їх можна або просто слухати або встановити на будильник.
- Мявчання кошенят;
- Кілька циферблатів, які можна налаштувати;
- Секундомір;
- Будильник;
- Календар;
- Зарядка по USB;
- Можна оновлювати прошивку по USB, не розбираючи годинник.
- Є індикатор заряду;
- Компоненти для збірки легко дістати;
- Корпус підходить для стандартних 20мм ремінців;
- Якщо під час експериментів з прошивками годинник завис, в корпусі передбачено отвір щоб голкою його перезавантажити.
- Водонепроникність до 0 метрів:) (можна трохи покращити якщо при збірці обробити плату лаком а корпус збирати з клеєм.)
- Годинник можна використовувати як поле для експериментів з ESP32-S2: акумулятор, 3 кнопки, пищалка, дисплей, 3 світлодіоди;
- Екран "Про годинник" містить інформацію про розробника та версію прошивки.




## Де брати комплектуючі


### Дисплей
<p align="center"><img src="Photos/5a768_electronic_circuits_gI_0_MemoryLCDPress.jpg" height="200"/> </p>

Дисплей потрібен, щоб відображати інформацію користувачу:)  <br>
Модель дисплея: **Sharp LS027B7DH01A**    <br>
Він звісно недешений, але мені дуже подобається як він виглядає. Його можна купити готовим, я купував тут:   <br>
https://www.aliexpress.us/item/1005005611440829.html  <br>


### Конектор дисплея
<p align="center"><img src="Photos/fpc.png" height="200"/> </p>

Для підключення цього дисплея. Я купував модулем, але можна пошукати і роз'єм окремо.  <br>
Модель: **FPC 10pin 0.5mm pitch** Я купував тут:    <br>
https://www.aliexpress.us/item/1005002288363000.html   <br>
https://www.aliexpress.us/item/10000348360254.html   <br>


### Буззер
<p align="center"><img src="Photos/buzzer.png" height="200"/> </p>

Буззер потрібен щоб пищати, грати мелодії.  <br>
Модель: **Buzzer SMD 7525-3.6V-passive**. Я купував тут:    <br>
https://www.aliexpress.us/item/1005003620083880.html   <br>
https://eu.mouser.com/ProductDetail/CUI-Devices/CMT-7525S-SMT-TR?qs=qCxwlXJ4fnw4ZHFvNib47A%3D%3D


### ESP32
<p align="center"><img src="Photos/1333_1.png" height="200"/> </p>

Я розпаював на комплектуючі плати **Lolin S2 Mini**, ось ссилка де я купував:  <br>
https://www.aliexpress.us/item/1005004438665554.html  <br>
  <br>
Якщо немає бажання розпаювати Lolin S2 Mini, можна купити комплектуху окремо:  <br>
<p align="center">
<img src="Photos/esp32s2.png" height=50"/> 
<img src="Photos/quartz.png" height=50"/> 
</p>

Чіп: **ESP32-S2, QFN56**  <br>
https://www.tme.eu/en/details/esp32s2/iot-wifi-bluetooth-modules/espressif/esp32-s2/   <br>
https://eu.mouser.com/ProductDetail/Espressif-Systems/ESP32-S2?qs=GBLSl2Akirtmlui%2FgtDaSQ%3D%3D   <br>
Кварц: **SMD 3225 4pin 40MHz**:  <br>
https://www.aliexpress.us/item/1005004689346275.html  <br>


### Модуль зарядки акумулятора
<p align="center"><img src="Photos/large_AOC777-1.jpg" height="200"/>  </p>

Розпаявши такий модуль можна дістати чіп **TP4056** , який відоповідає за зарядку акума. Датащит:  <br>
https://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/TP4056.pdf  <br>
Якщо нема бажання розпаювати модуль, можна знайти окремо:  <br>
<p align="center">
<img src="Photos/tp4056.png" height=50"/> 
<img src="Photos/usb.png" height=50"/> 
</p>

Чіп: **TP4056**  <br>
https://www.alibaba.com/product-detail/TC4056A-SOP-8-is-compatible-with_1600806443781.html  <br>
Роз'єм: **TYPE-C-31-M-12**  <br>
https://jlcpcb.com/partdetail/Korean_HropartsElec-TYPE_C_31_M12/C165948  <br>


### Електролюмінесцентна панель
<p align="center"><img src="Photos/el.png" height="200"/>  </p>

Електролюмінесцентна панель (EL панель) є складовою частиною системи підсвітки. Це панель, що світиться під впливом високої напруги і частоти. Також їх називають світлоконденсаторами (LEC). Це просто цікаві факти щоб краще розуміти що воно таке.  <br>
Фішка цього типу підсвітки в тому, що панель є дуже тонка, світить дуже рівномірно приємним синім кольором, а також її можна нарізати на фрагменти потрібної форми. Але після порізки складною є задача підключення до панелі. Я для підключення використовував одиночні жили дроти а також токопровідний клей.  <br>
Мінуси: будучи розташована за дисплеєм який тут використовується, її ледь-ледь видно, але вночі цього достатньо. Також трохи пищить при роботі. А ще для її роботи потрібна досить складна схема драйвера.  <br>
Раджу купувати синій колір, тому що EL панель натівно такого кольору, а в інші кольори її тупо фарбують, а фарба поглинає частину світла. Якщо прийшла пофарбована панель, її можна очистити від фарби лезом.  <br>
Ось де я купував:  <br>
https://www.aliexpress.us/item/1005003971896012.html  <br>


### Драйвер електролюмінесцентної панелі
<p align="center"><img src="Photos/h857.png" height="200"/>  </p>

Модель: **Microchip HV857MG MSOP8**   <br>
Датащит:   <br>
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/HV857-High-Voltage-Low-Noise-EL-Lamp-Driver-IC-Data-Sheet-DS20005683.pdf   <br>
Я купував тут:  <br>
https://www.aliexpress.us/item/1005005512523052.html  <br>



### Обвязка драйвера електролюмінесцентної панелі
<p align="center">
<img src="Photos/capacitor.png" height=50"/> 
<img src="Photos/capacitorset.png" height=50"/> 
<img src="Photos/es1d.png" height=50"/> 
<img src="Photos/l.png" height=50"/> 
</p>

Насправді я в цій мелочовці взагалі не шарю, причому навіть деякі елементи замовив не ті що треба було... Але у мене воно запрацювало. Тому я кладу тут ссилки на те що я конкретно замовляв.

Модель: **Capacitor SMD 1206 10NF X7R 200V**   <br>
https://www.aliexpress.us/item/1005004628006766.html   <br>

Модель: **1206 320PCS 16Value**   <br>
https://www.aliexpress.us/item/1005006124283234.html   <br>

Модель: **Diode ES1D**   <br>
https://www.aliexpress.us/item/1005005254835443.html   <br>

Модель: **SMD Inductor 1210 3225 22UH**   <br>
https://www.aliexpress.us/item/32981310345.html   <br>



### Клей для електролюмінесцентної панелі
<p align="center"><img src="Photos/mechanic.png" height="200"/>  </p>

Модель: **MECHANIC MCN-DJ002 Conductive Adhesive Glue Silver**   <br>
https://www.aliexpress.us/item/1005002369819447.html   <br>



## Процес збірки та налагодження
<p align="center"><img src="Photos/preheater.png" height=80"/> </p>
Мені дуже допомогла в пайці така прикольна паяльна станція:
https://www.aliexpress.us/item/1005005609080807.html


<p align="center"><img src="Photos/breakout.png" height=80"/> </p>
На етапі налагодження може допомогти такий Breakout Board який підійде для ESP32-S2 (нагадую, там корпус QFN56)
https://www.aliexpress.us/item/1005002805132028.html


## Інструменти, які я використовував при проектуванні:
- Проектування плати та схеми: **KiCad 7**;
- Програмування: **Arduino IDE 2.2.1**;
- Розширення плати: **esp32 by Espressif v2.0.11, ESP32-S2** (В списку плат оберіть "Lolin S2 Mini");
- Конвертування іконок: **Програма LCD Assistant**. (Виберіть Horizontal)



## How to add own melodies:
To make melodies I used tool by ShivamJoker which I modified to generate my melody format.
Use tool in Software\MIDI-to-Arduino-main. 
Add melody array to MelodyPlayer.ini file and add melody to melodies array.



## Known libraries conflicts:
Temperature measurement is hanging system if after Wi-Fi usage it were not initialized.

##Steel Marking Process:
https://youtube.com/shorts/PZIMB8fdEP8?si=xofTaj4oOFBRnQQU


## References

- Big repository of phone melodies where I found some melodies
http://onj3.andrelouis.com/phonetones/unzipped/





##Історія проекту
- 2023 рік, я десь наткнувся на відео про sharp memory lcd зрозумів, що дуже хочу собі такий дисплей. Замовив на алі.
- 

