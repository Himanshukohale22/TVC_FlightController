# STM32g0 neclo board development for TCV Flight controller.

This repository contains the firmware for Hardware desinged specifically for flight controller with STM32G071RB microcontroller and some other sensors and electronics components.


### Project Flow

```
├── app
│   ├── gps.c
│   ├── imu.c
│   ├── led.c
│   ├── pub.c
│   ├── SD_card.c
│   ├── servo.c
│   ├── sub.c
│   └── ThrustVector.c
├── draw
│   └── SoftwareArchitecture.drawio.png
├── FreeRTOS
│   ├── CMakeLists.txt
│   ├── croutine.c
│   ├── cspell.config.yaml
│   ├── event_groups.c
│   ├── examples
│   ├── GitHub-FreeRTOS-Kernel-Home.url
│   ├── History.txt
│   ├── include
│   ├── LICENSE.md
│   ├── list.c
│   ├── manifest.yml
│   ├── MISRA.md
│   ├── portable
│   ├── queue.c
│   ├── Quick_Start_Guide.url
│   ├── README.md
│   ├── stream_buffer.c
│   ├── tasks.c
│   └── timers.c
├── include
├── lib
├── libopencm3
│   ├── COPYING.GPL3
│   ├── COPYING.LGPL3
│   ├── doc
│   ├── HACKING
│   ├── HACKING_COMMON_DOC
│   ├── include
│   ├── ld
│   ├── lib
│   ├── locm3.sublime-project
│   ├── Makefile
│   ├── mk
│   ├── README.md
│   ├── scripts
│   └── tests
├── Makefile
├── minimal.ld
├── Readme.md
└── src
    ├── main.c
    ├── main.d
    └── main.o
```
