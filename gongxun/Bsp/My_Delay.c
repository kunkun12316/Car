#include "My_Delay.h"


//us级延时设计
__IO float usDelayBase;
void PY_usDelayTest(void)
{
    __IO uint32_t firstms, secondms;
    __IO uint32_t counter = 0;

    firstms = HAL_GetTick()+1;
    secondms = firstms+1;

    while(uwTick!=firstms) ;

    while(uwTick!=secondms) counter++;

    usDelayBase = ((float)counter)/1000;
}

void PY_Delay_us_t(uint32_t Delay)
{
    __IO uint32_t delayReg;
    __IO uint32_t usNum = (uint32_t)(Delay*usDelayBase);

    delayReg = 0;
    while(delayReg!=usNum) delayReg++;
}

//us级延时设计优化
void PY_usDelayOptimize(void)
{
    __IO uint32_t firstms, secondms;
    __IO float coe = 1.0;

    firstms = HAL_GetTick();
    PY_Delay_us_t(1000000) ;
    secondms = HAL_GetTick();

    coe = ((float)1000)/(secondms-firstms);
    usDelayBase = coe*usDelayBase;
}

//us延时函数优化
void PY_Delay_us(uint32_t Delay)
{
    __IO uint32_t delayReg;

    __IO uint32_t msNum = Delay/1000;
    __IO uint32_t usNum = (uint32_t)((Delay%1000)*usDelayBase);

    if(msNum>0) HAL_Delay(msNum);

    delayReg = 0;
    while(delayReg!=usNum) delayReg++;
}

//ms延时函数设计,基于us延时函数
void PY_Delay_ms(uint32_t Delay)
{
    while(Delay)
    {
        PY_Delay_us(1000);
        Delay--;
    }
}


//_________________________________________________________________________________________________

static uint8_t fac_us = 0; // 计数器因子
void Delay_Init(uint8_t SysCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8); // 设置SysTick时钟源：HCLK
    fac_us = SysCLK / 8;                                  // 设置计数器因子
}

/// @brief nus延时
/// @param nus 延时的nus数
void Delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us - 1;         // 计数值加载
    SysTick->VAL = 0x00;                      // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始计数
    do
    {
        temp = SysTick->CTRL;                     // 读取控制寄存器状态
    } while ((temp & 0x01) && !(temp & (1 << 16))); // temp&0x01:定时器使能，!(temp&(1<<16)):定时器计数值不为0
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      // 关闭计数
    SysTick->VAL = 0x00;                            // 清空计数器
}

/// @brief nms延时
/// @param nus 延时的ms数
void Delay_ms(uint32_t nms)
{
    uint32_t repeat = nms / 540;
    uint32_t remain = nms % 540;
    while (repeat)
    {
        Delay_us(540 * 1000); // 延时 540 ms
        repeat--;
    }
    if (remain)
    {
        Delay_us(remain * 1000); // 延时remain ms
    }
}
