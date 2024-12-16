    这是STM32官方提供的3个启动文件，分别适用于不同容量的STM32芯片。
    这三个文件为：
        startup_stm32f10x_ld.s
        startup_stm32f10x_md.s
        startup_stm32f10x_hd.s
    其中，ld.s适用于小容量 产品；md.s适用于中等容量产品；hd适用于大容量产品；
    这里的容量是指FLASH的大小.判断方法如下：
       小容量：FLASH≤32K
       中容量：64K≤FLASH≤128K
       大容量：256K≤FLASH
    所以，请大家根据自己所用的STM32芯片选择不同的.s文件。

    Translate:

    This is the official STM32 provides three startup files, respectively, for different capacity of STM32 chips.
    These three files are:
        startup_stm32f10x_ld.s
        startup_stm32f10x_md.s
        startup_stm32f10x_hd.s
    Among them, ld.s is suitable for small capacity products; md.s is suitable for medium capacity products; hd is suitable for large capacity products;
    Here, the capacity refers to the size of FLASH. The judgment method is as follows:
        Small capacity: FLASH≤32K
        Medium capacity: 64K≤FLASH≤128K
        Large capacity: 256K≤FLASH
    So, please choose different .s files according to the STM32 chip you are using.
