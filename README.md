
<!-- For new line: \ -->
<!-- Picture: <img src="Photos/" width="300"/>  -->
<!-- New Line: "  <br>"  -->

# Про проект DRM Watch v3

<p align="center">
<img src="Photos/DRM_welcome.jpg" width="500"/>
</p>

Наручний годинник на базі ESP32-S2 та SHARP Memory LCD.  <br>
А все починалось з того, що мені сподобався дисплей...  <br>
Тут можна знайти інфу і інструкції щоб зібрати собі такий годинник.  <br>
Посилання що наведено в цьому тексті є актуальними станом на 2024-04. Якщо якісь товари будуть вже не актуальні, то шукайте за фотками))  <br>
  <br>



<p align="center">
<img src="Photos/cad-angle.png" width="200"/>
<img src="Photos/cad-front.png" width="200"/>
<img src="Photos/cad-side.png" width="200"/>
</p>

<p align="center">
<img src="Photos/photo_2024-02-04_23-56-24.jpg" width="200"/>
<img src="Photos/photo_2024-02-04_16-33-34.jpg" width="200"/>
<img src="Photos/photo_2024-02-04_16-33-34 (2).jpg" width="200"/>
</p>


## Особливості годинника
- Стабільно працює і підходить для щоденного користування.
- Дисплей завжди показує час;
- Є дуже тускла але дуже приємна нічна підсвітка дисплея;
- Є ліхтарик, білий яскравий та червоний нічний;
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
Готових в продажу не існує, тому якщо є бажання отримати такий годинник, можна зібрати самому.

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


### Акумулятор
<p align="center"><img src="Photos/battery.png" height="200"/> </p>

Якщо потрібно щоб пристрій працював автономно, то потрібен акумулятор.  <br>
Корпус та плата спроектовано під літій-іонний акумулятор такої форми: 35*25*5mm. Звісно, підійде будь-який і меншого розміру, але матиме меншу ємність.  <br>
Модель яка найкраще підходить: **lipo 502535**. Я купував тут:    <br>
https://prom.ua/ua/p1286391849-akkumulyator-kontrollerom-universalnyj.html   <br>

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
<p align="center">
<img src="Photos/large_AOC777-1.jpg" height="200"/>  
</p>

Щоб була можливість заряджати акумулятор. Причому якщо акума не буде - годинник не працюватиме навіть на зарядці.   <br>
Тому якщо потрібно обійти модуль зарядки, можна замкнути перемичку біля мікросхеми заряду на платі (вона вона розташована біля самого краю плати) - вона подасть напряму напругу з USB порта замість напруги батареї. Основне не залишити перемичку зі вставленим акумом, щоб не отримати фаєр-шоу)))   <br>
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


### SMD Резистори
<p align="center"><img src="Photos/resistors.png" height="200"/>  </p>

Я робив плату під резистори **SMD 0603** (переважно), їх я брав з такого набору:   <br>
https://www.aliexpress.us/item/1005002364437129.html   <br>


### SMD Конденсатори
<p align="center"><img src="Photos/capacitorset.png" height="200"/>  </p>

Я робив плату під конденсатори **SMD 0603** (переважно), їх я брав з такого набору:   <br>
https://www.aliexpress.us/item/1005006124283234.html   <br>


### RTC модуль
<p align="center">
<img src="Photos/rtc.png" height="200"/>   
<img src="Photos/rtcchip.png" height="200"/>  
</p>

Використовується мікросхема **Analog Devices DS3231**, я її купував в складі такого модуля:   <br>
https://www.aliexpress.us/item/32822420722.html   <br>
Але можна також знайти мікросхеми окремо. Ось ще трохи інфи по ним:   <br>
https://octopart.com/ds3231-maxim+integrated-14739425
https://www.analog.com/en/products/ds3231.html   <br>
https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf   <br>


### Світлодіод ліхтарика
<p align="center">
<img src="Photos/020led.png" height="200"/>  
<img src="Photos/lcd-backlight.png" height="200"/>  
</p>

Ці світлодіоди я діставав виключно з підсвітки старих телефонів, бо їх дисплеїв у мене багато.   <br>
Але я знайшов їх типорозмір щоб можна було замовити окремо: **020 SMD Side View LED White 6000K**.   <br>
Знайшов що тут можна замовити:  <br>
https://www.aliexpress.us/item/32419907074.html   <br>


### Кнопки
<p align="center"><img src="Photos/buttons.png" height="200"/>  </p>

Я так і не знайшов як їх можна назвати, тому шукаємо по фото і цьому тексту: **2*4*3.5mm Micro SMD Tact Switch Side Button**  <br>
https://www.aliexpress.us/item/32880903601.html    <br>


### Місця для гвинтів

Такі можна знайти на материнці DJI Mavic 3. Під гвинт М1.2  <br>
Щось схоже:  <br>
Обирати SMTSO-M1.2-2  <br>
https://www.aliexpress.com/i/1005005125674149.html  <br>

https://www.swaco.com/SMTSO-M1.2-2ET-PEM-ReelFast  <br>

SMTSO-M1.2-2ET  <br>
https://eu.mouser.com/ProductDetail/PEM/SMTSO-M1.2-2ET?qs=l4Gc20tDgJKS2GEfMx1Yrw%3D%3D  <br>



### Електролюмінесцентна панель
<p align="center"><img src="Photos/el.png" height="200"/>  </p>

Цей елемент можна ставити або не ставити за бажанням, залежно від того чи потрібна вам нічна підсвітна екрана.   <br>
Електролюмінесцентна панель (EL панель) є складовою частиною системи підсвітки. Це панель, що світиться під впливом високої напруги і частоти. Також їх називають світлоконденсаторами (LEC). Це просто цікаві факти щоб краще розуміти що воно таке.  <br>
Фішка цього типу підсвітки в тому, що панель є дуже тонка, світить дуже рівномірно приємним синім кольором, а також її можна нарізати на фрагменти потрібної форми. Але після порізки складною є задача підключення до панелі. Я для підключення використовував одиночні жили дроти а також токопровідний клей.  <br>
Мінуси: будучи розташована за дисплеєм який тут використовується, її ледь-ледь видно, але вночі цього достатньо. Також трохи пищить при роботі. А ще для її роботи потрібна досить складна схема драйвера.  <br>
Раджу купувати синій колір, тому що EL панель натівно такого кольору, а в інші кольори її тупо фарбують, а фарба поглинає частину світла. Якщо прийшла пофарбована панель, її можна очистити від фарби лезом.  <br>
Ось де я купував:  <br>
https://www.aliexpress.us/item/1005003971896012.html  <br>


### Драйвер електролюмінесцентної панелі
<p align="center"><img src="Photos/h857.png" height="200"/>  </p>

Цей елемент можна ставити або не ставити за бажанням, залежно від того чи потрібна вам нічна підсвітна екрана.   <br>
Це та мікросхема, яка і формує змінну високу напругу для електролюмінесцентної панелі.   <br>
Схему її підключення я не розумію, просто зібрав по схемі і воно запрацювало))   <br>
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

Цей елемент можна ставити або не ставити за бажанням, залежно від того чи потрібна вам нічна підсвітка екрана.   <br>
Насправді я в цій мелочовці взагалі не шарю, причому навіть деякі елементи замовив не ті що треба було... Але у мене воно запрацювало. Тому я кладу тут ссилки на те що я конкретно замовляв.   <br>

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

Цей елемент можна ставити або не ставити за бажанням, залежно від того чи потрібна вам нічна підсвітка екрана.   <br>
Модель: **MECHANIC MCN-DJ002 Conductive Adhesive Glue Silver**   <br>
https://www.aliexpress.us/item/1005002369819447.html   <br>




## Корпус
<p align="center">
<img src="Photos/case-front.png" height="250"/>  
<img src="Photos/case-back.png" height="250"/>
<img src="Photos/case-buttons.png" height="100"/>
</p>

Корпус спроектовано для простоти збірки та простоти друку на FDM 3D принтері. При цьому можна також замовити друк металом або фрезерування.  <br>
Малював корпус в Fusion 360, якщо потрібно щось змінювати, відкривайте файли `.f3d`.

## Плата
<p align="center">
<img src="Photos/board1.png" height="300"/>  
<img src="Photos/board3.png" height="300"/>  
</p>
<p align="center">
<img src="Photos/board2.png" height="150"/>  
<img src="Photos/board4.png" height="150"/>  
<img src="Photos/board5.png" height="150"/>  
</p>

В папці можна знайти плати різних ревізій, коротко про ревізії:  <br>
**Rev 1:** конденсатори розміщені надто далеко від контроллера, конденсаторів мало, 
немає елементів що формують EN для контроллера, нема RTC мікросхеми, антена на платі неефективна, 
розпіновка в деяких моментах відрізняється від нової ревізії, елементи розміщені не плотно, немає ніякої підсвітки.  <br>
**Rev 2:** вісутні підтяжки RTC SCL та SDA. Елементи розміщені не плотно, підсвітна світлодіодна і неефективна.  <br>
**Rev 3:** Помилок по схемі наче поки немає, я зібрав і воно одразу запрацювало. 
Підсвітка електролюмінесцентна з драйвером. Компоненти розміщені плотно. Можна поставити великий акум на 500мАг.  <br>
<br>
Щоб замовити собі плату, можна скористатись файлами в папках GERBER. 
Ці файли достатньо завантажити на сайт не виготовляють плати, я замовляв у **jlcpcb.com**. 
Там одразу має бути видно як виглядатиме плата.   <br>
Якщо потрібно щось поміняти - ставимо KiKad, відкриваєм проект `\Board\_DRM_Watch_v3` і можна міняти.   <br>
Експортувати Gerber на Drl можна в редакторі плат KiKad.  <br>
Також в схемі в проекті я до кожного елемента пододавав інфу про те де його можна замовити (якщо знаю звісно)  <br>



## Процес збірки та налагодження
<p align="center"><img src="Photos/preheater.png" height=80"/> </p>

Мені дуже допомогла в пайці така прикольна паяльна станція:  <br>
https://www.aliexpress.us/item/1005005609080807.html  <br>


<p align="center"><img src="Photos/breakout.png" height=80"/> </p>

На етапі налагодження може допомогти такий **QFN56 Breakout Board** який підійде для ESP32-S2 (нагадую, там корпус QFN56)  <br>
https://www.aliexpress.us/item/1005002805132028.html  <br>

**Uretan 71**  <br>
Для покриття плат лаком я використовував таку штуку як **Uretan 71**. Наносив за допомогою маленького пензля в два шари.   <br>


## Інструменти, які я використовував при проектуванні:
- Проектування плати та схеми: **KiCad 7**;
- Програмування: **Arduino IDE 2.2.1**;
- Розширення плати: **esp32 by Espressif v2.0.11, ESP32-S2** (В списку плат оберіть "Lolin S2 Mini");
- Конвертування іконок: **Програма LCD Assistant**. (Виберіть Horizontal)



## Прошивка
Прошивка написана в **Arduino IDE 2.2.1** з використанням розширення плат **esp32 by Espressif v2.0.11, ESP32-S2**, компілювати треба під плату **Lolin S2 Mini**.   <br>
Кожен режим в прошивці (поточний екран, mode) - це комплект функцій які займаються відмальовуванням екрану та обробкою кнопок. Кожен режим може робити по суті що завгодно.   <br>
Основний спосіб навігації в прошивці це `Runnable modeSetup, modeLoop, modeButtonUp, modeButtonCenter, modeButtonDown, modeButtonUpLong, modeButtonCenterLong, modeButtonDownLong`.    <br>
Ці змінні в кожен момент часу містять посилання на функції які відповідають поточному режиму (екрану). 
Коли потрібно перемкнути режим (поточний екран), функція `modeSetup` відновідного режиму має перезаписати ці посилання, 
і відновідно `loop` викликатиметься вже інший, малюватиметься інша інфа, користувач бачитиме інший екран та взаємодіятиме з ним.   <br>
   <br>

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

- Vibration implementation
https://makeabilitylab.github.io/physcomp/advancedio/vibromotor.html





##Історія проекту
- 2023 рік, я десь наткнувся на відео про sharp memory lcd зрозумів, що дуже хочу собі такий дисплей. Замовив на алі.
- 

