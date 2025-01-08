#include "sys.h"

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr)
{
	__ASM volatile (
	        "MSR MSP, %0\n"  // Set Main Stack Pointer
	        "BX LR"          // Branch back
	        :
	        : "r" (addr)     // Input operand: addr
			  );
}

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //Turn on the auxiliary clock //��������ʱ��	   
	AFIO->MAPR&=0XF8FFFFFF; //Clear MAPR [26:24] //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //Set the JTAG mode //����jtagģʽ
} 

