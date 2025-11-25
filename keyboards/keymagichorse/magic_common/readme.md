

这个wireless_lib必须得QMK在2025Q3以上的版本才能用，否则得改usb_main.c hook一下才能用。

Very few codes are borrowed from https://www.keychron.com
有个别代码是借鉴keychron，个别是QMK原生修改的
低功耗代码F4基本都是网上有的，基于实际修改适合QMK的
现在所有的配置都移动到wireless_config.h这个文件，keyboards\键盘\config.h可以覆盖

magic_task 和 magic_common_init 这里个函数必须丢到这两个函数里面

```c
void board_init(void) 
{
    magic_common_init();
}
void housekeeping_task_kb(void) {
    magic_task();
}
```







