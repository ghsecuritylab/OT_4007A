
#include <common.h>
#include <asm/io.h>

#include <asm/arch/mtk_auxadc_sw.h>
#include <asm/arch/mtk_auxadc_hw.h>

///////////////////////////////////////////////////////////////////////////////////////////
//// Define
static int adc_auto_set =0;

typedef unsigned short  kal_uint16;

#define DRV_Reg(addr)               (*(volatile kal_uint16 *)(addr))
#define DRV_WriteReg(addr,data)     ((*(volatile kal_uint16 *)(addr)) = (kal_uint16)(data))

#define DRV_ClearBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp &=~(data);\
   DRV_WriteReg(addr,temp);\
}

#define DRV_SetBits(addr,data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp |= (data);\
   DRV_WriteReg(addr,temp);\
}

#define DRV_SetData(addr, bitmask, value)     {\
   kal_uint16 temp;\
   temp = (~(bitmask)) & DRV_Reg(addr);\
   temp |= (value);\
   DRV_WriteReg(addr,temp);\
}

#define AUXADC_DRV_ClearBits16(addr, data)           DRV_ClearBits(addr,data)
#define AUXADC_DRV_SetBits16(addr, data)             DRV_SetBits(addr,data)
#define AUXADC_DRV_WriteReg16(addr, data)            DRV_WriteReg(addr, data)
#define AUXADC_DRV_ReadReg16(addr)                   DRV_Reg(addr)
#define AUXADC_DRV_SetData16(addr, bitmask, value)   DRV_SetData(addr, bitmask, value)

#define AUXADC_DVT_DELAYMACRO(u4Num)                                     \
{                                                                        \
    unsigned int u4Count = 0 ;                                                 \
    for (u4Count = 0; u4Count < u4Num; u4Count++ );                      \
}

#define AUXADC_SET_BITS(BS,REG)       ((*(volatile u32*)(REG)) |= (u32)(BS))
#define AUXADC_CLR_BITS(BS,REG)       ((*(volatile u32*)(REG)) &= ~((u32)(BS)))

#define VOLTAGE_FULL_RANGE 	2500 // VA voltage
#define ADC_PRECISE 		4096 // 12 bits

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//// Internal API
inline static void mt65xx_ADC_2G_power_up(void)
{
    //2010/07/27: mt6573, the ADC 2G power on is controlled by APMCU_CG_CLR0
    //#define PDN_CLR0 (0xF7026308)  
    #define PDN_CLR0 (0x70026308)  
    unsigned int poweron = 1 << 4;
    AUXADC_SET_BITS(poweron, PDN_CLR0);
}

inline static void mt65xx_ADC_2G_power_down(void)
{
    //2010/07/27: mt6573, the ADC 2G power on is controlled by APMCU_CG_SET0
    //#define PDN_SET0 (0xF7026304)  
    #define PDN_SET0 (0x70026304)  
    unsigned int poweroff = 1 << 4;
    AUXADC_SET_BITS(poweroff, PDN_SET0);
}

inline static void mt65xx_ADC_3G_power_up(void)
{
    //2010/07/27: mt6573, the ADC 3G power on is controlled by APMCU_CG_CLR0
    //#define PDN_CLR0 (0xF7026308)  
    #define PDN_CLR0 (0x70026308)  
    unsigned int poweron = 1 << 13;
    AUXADC_SET_BITS(poweron, PDN_CLR0);
}

inline static void mt65xx_ADC_3G_power_down(void)
{
    //2010/07/27: mt6573, the ADC 3G power on is controlled by APMCU_CG_SET0
    //#define PDN_SET0 (0xF7026304)  
    #define PDN_SET0 (0x70026304)  
    unsigned int poweroff = 1 << 13;
    AUXADC_SET_BITS(poweroff, PDN_SET0);
}
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//// Common API

int auxadc_test(void ) 
{
   int i = 0, data[4] = {0,0,0,0};
   int res =0;
   int rawdata=0;
   

      for (i = 0; i < 14; i++) 
      {
        //printk("[adc_driver]: i=%d\n",i);
       
		res = IMM_GetOneChannelValue(i,data,&rawdata);
		if(res < 0)
		{ 
			   printf("[adc_uboot]: get data error\n");
			   break;
			   
		}
		else
		{
		       printf("[adc_uboot]: channel[%d]raw =%d\n",i,rawdata);
		       printf("[adc_uboot]: channel[%d]=%d.%.02d \n",i,data[0],data[1]);
			  
		}
			
      } 


   return 0;
}

//fix FR440426 start
//step1 check con3 if auxadc is busy
//step2 clear bit
//step3  read channle and make sure old ready bit ==0
//step4 set bit  to trigger sample
//step5  read channle and make sure  ready bit ==1
//step6 read data

int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata)
{
   unsigned int channel[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int idle_count =0;
   int data_ready_count=0;

   //step1 check con3 if auxadc is busy
   while ((*(volatile u16 *)AUXADC_CON3) & 0x01) 
   {
       printf("[adc_api]: wait for module idle\n");
       udelay(100000);
	   idle_count++;
	   if(idle_count>30)
	   {
	      //wait for idle time out
	      printf("[adc_api]: wait for aux/adc idle time out\n");
	      return -1;
	   }
   } 
   // step2 clear bit
   if(0 == adc_auto_set)
   {
	   //clear bit
	   *(volatile u16 *)AUXADC_CON1 = *(volatile u16 *)AUXADC_CON1 & (~(1 << dwChannel));
   }
   

   //step3  read channle and make sure old ready bit ==0
   while ((*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & (1<<12)) 
   {
       printf("[adc_api]: wait for channel[%d] ready bit clear\n",dwChannel);
       udelay(10000);
	   data_ready_count++;
	   if(data_ready_count>30)
	   {
	      //wait for idle time out
	      printf("[adc_api]: wait for channel[%d] ready bit clear time out\n",dwChannel);
	      return -2;
	   }
   }
  
   //step4 set bit  to trigger sample
   if(0==adc_auto_set)
   {  
	  *(volatile u16 *)AUXADC_CON1 = *(volatile u16 *)AUXADC_CON1 | (1 << dwChannel);
   }
   //step5  read channle and make sure  ready bit ==1
   udelay(1000);//we must dealay here for hw sample cahnnel data
   while (0==((*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & (1<<12))) 
   {
       printf("[adc_api]: wait for channel[%d] ready bit ==1\n",dwChannel);
       udelay(10000);
	 data_ready_count++;
	 if(data_ready_count>30)
	 {
	      //wait for idle time out
	      printf("[adc_api]: wait for channel[%d] data ready time out\n",dwChannel);
	      return -3;
	 }
   }
   ////step6 read data
   
   channel[dwChannel] = (*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & 0x0FFF;
   if(NULL != rawdata)
   {
      *rawdata = channel[dwChannel];
   }
   //printk("[adc_api: imm mode raw data => channel[%d] = %d\n",dwChannel, channel[dwChannel]);
   //printk("[adc_api]: imm mode => channel[%d] = %d.%d\n", dwChannel, (channel[dwChannel] * 250 / 4096 / 100), ((channel[dwChannel] * 250 / 4096) % 100));
   data[0] = (channel[dwChannel] * 250 / 4096 / 100);
   data[1] = ((channel[dwChannel] * 250 / 4096) % 100);
	 
   return 0;
   
}
//fix FR440426 end
#if 0
int IMM_GetOneChannelValue(int dwChannel, int deCount)
{
    unsigned int u4Sample_times = 0;
    unsigned int dat = 0;
	unsigned int u4channel[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned int adc_esult=0;

    /* Enable ADC power bit */    
    mt65xx_ADC_2G_power_up();
	mt65xx_ADC_3G_power_up();

    /* Initialize ADC control register */
    AUXADC_DRV_WriteReg16(AUXADC_CON0, 0);
    AUXADC_DRV_WriteReg16(AUXADC_CON1, 0);    
    AUXADC_DRV_WriteReg16(AUXADC_CON2, 0);    
    AUXADC_DRV_WriteReg16(AUXADC_CON3, 0);   

    do
    {
        //pmic_adc_vbat_enable(KAL_TRUE);		// move to the whom driver
        //pmic_adc_isense_enable(KAL_TRUE); 	// move to the whom driver

        AUXADC_DRV_WriteReg16(AUXADC_CON1, 0);        
        AUXADC_DRV_WriteReg16(AUXADC_CON1, 0x1FFF);
         
        AUXADC_DVT_DELAYMACRO(1000);

        /* Polling until bit STA = 0 */
        while (0 != (0x01 & AUXADC_DRV_ReadReg16(AUXADC_CON3)));          

        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT0);        
        u4channel[0]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT1);        
        u4channel[1]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT2);        
        u4channel[2]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT3);        
        u4channel[3]  += (dat & 0x0FFF);   
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT4);
        u4channel[4]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT5);
        u4channel[5]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT6);
        u4channel[6]  += (dat & 0x0FFF);  
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT7);
        u4channel[7]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT8);
        u4channel[8]  += (dat & 0x0FFF);    
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT9);
        u4channel[9]  += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT10);
        u4channel[10] += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT11);
        u4channel[11] += (dat & 0x0FFF);
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT12);
        u4channel[12] += (dat & 0x0FFF);		
        dat = AUXADC_DRV_ReadReg16(AUXADC_DAT13);
        u4channel[13] += (dat & 0x0FFF);
        
        u4Sample_times++;
    }
    while (u4Sample_times < deCount);

    /* Disable ADC power bit */    
    mt65xx_ADC_2G_power_down();
	mt65xx_ADC_3G_power_down();

    #if 0
    printf("BAT_GetVoltage : channel_0  = %d / %d \n", u4channel[0], u4Sample_times );
    printf("BAT_GetVoltage : channel_1  = %d / %d \n", u4channel[1], u4Sample_times );
    printf("BAT_GetVoltage : channel_2  = %d / %d \n", u4channel[2], u4Sample_times );
    printf("BAT_GetVoltage : channel_3  = %d / %d \n", u4channel[3], u4Sample_times );
    printf("BAT_GetVoltage : channel_4  = %d / %d \n", u4channel[4], u4Sample_times );
    printf("BAT_GetVoltage : channel_5  = %d / %d \n", u4channel[5], u4Sample_times );
    printf("BAT_GetVoltage : channel_6  = %d / %d \n", u4channel[6], u4Sample_times );
    printf("BAT_GetVoltage : channel_7  = %d / %d \n", u4channel[7], u4Sample_times );
    printf("BAT_GetVoltage : channel_8  = %d / %d \n", u4channel[8], u4Sample_times );
    printf("BAT_GetVoltage : channel_9  = %d / %d \n", u4channel[9], u4Sample_times );
    printf("BAT_GetVoltage : channel_10 = %d / %d \n", u4channel[10], u4Sample_times );
    printf("BAT_GetVoltage : channel_11 = %d / %d \n", u4channel[11], u4Sample_times );
    printf("BAT_GetVoltage : channel_12 = %d / %d \n", u4channel[12], u4Sample_times );
    printf("BAT_GetVoltage : channel_13 = %d / %d \n", u4channel[13], u4Sample_times );	
    #endif

	/* Value averaging  */ 
    u4channel[0]  = u4channel[0]  / deCount;
    u4channel[1]  = u4channel[1]  / deCount;
    u4channel[2]  = u4channel[2]  / deCount;
    u4channel[3]  = u4channel[3]  / deCount;
    u4channel[4]  = u4channel[4]  / deCount;
    u4channel[5]  = u4channel[5]  / deCount;
    u4channel[6]  = u4channel[6]  / deCount;
    u4channel[7]  = u4channel[7]  / deCount;
    u4channel[8]  = u4channel[8]  / deCount;
    u4channel[9]  = u4channel[9]  / deCount;
    u4channel[10] = u4channel[10] / deCount;
    u4channel[11] = u4channel[11] / deCount;
    u4channel[12] = u4channel[12] / deCount;
    u4channel[13] = u4channel[13] / deCount;

	adc_esult = ((u4channel[dwChannel]*VOLTAGE_FULL_RANGE)/ADC_PRECISE);

	return adc_esult;
	
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////

