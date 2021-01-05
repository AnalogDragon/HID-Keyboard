# HID Keyboard

使用STM32 hal库的HID键盘;

通过USB组合设备(Composite Device)，实现标准键盘与多媒体控制功能;

矩阵键盘尺寸为 12 input * 9 output;

DMA+SPI组合使用模拟one wire协议，用于按键背光;

通过电压预估电池寿命;

与蓝牙模块通过uart连接;

蓝牙模块可以任选NRF52832板;

