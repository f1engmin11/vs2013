#define Bit_0   (1<<0)
#define Bit_1   (1<<1)
#define Bit_2   (1<<2)
#define Bit_3   (1<<3)
#define Bit_4   (1<<4)
#define Bit_5   (1<<5)
#define Bit_6   (1<<6)
#define Bit_7   (1<<7)

#define Bit_8    (1<<8)
#define Bit_9    (1<<9)
#define Bit_10   (1<<10)
#define Bit_11   (1<<11)
#define Bit_12   (1<<12)
#define Bit_13   (1<<13)
#define Bit_14   (1<<14)
#define Bit_15   (1<<15)

#define Bit_16   (1<<16)
#define Bit_17   (1<<17)
#define Bit_18   (1<<18)
#define Bit_19   (1<<19)
#define Bit_20   (1<<20)
#define Bit_21   (1<<21)
#define Bit_22   (1<<22)
#define Bit_23   (1<<23)

#define Bit_24   (1<<24)
#define Bit_25   (1<<25)
#define Bit_26   (1<<26)
#define Bit_27   (1<<27)
#define Bit_28   (1<<28)
#define Bit_29   (1<<29)
#define Bit_30   (1<<30)
#define Bit_31   (1<<31)

typedef enum _BOOL { FALSE = 0, TRUE} BOOL;
typedef unsigned char INT8U;
typedef signed   char INT8S;
typedef unsigned int  INT16U;
typedef signed   int  INT16S;
typedef unsigned long INT32U;
typedef signed   long INT32S;
typedef float         FP32;
typedef double        FP64;



#define CPLD_KeyScan_Reg         (*((volatile unsigned char *) 0x82010042))
#define CPLD_KeyDate_Reg         (*((volatile unsigned char *) 0x82010044))

//#define DCU_CMD_PORT           (*((volatile unsigned char *) 0x82010032))

#define CPLD_XAXIS_L_Reg      (*((volatile unsigned char *) 0x82010000))
#define CPLD_XAXIS_H_Reg      (*((volatile unsigned char *) 0x82010002))

#define CPLD_ZAXIS_L_Reg      (*((volatile unsigned char *) 0x82010004))
#define CPLD_ZAXIS_H_Reg      (*((volatile unsigned char *) 0x82010006))

#define CPLD_Timer_EN_Reg     (*((volatile unsigned char *) 0x82010010))
#define CPLD_Timer_L_Reg      (*((volatile unsigned char *) 0x82010012))
#define CPLD_Timer_H_Reg      (*((volatile unsigned char *) 0x82010014))

#define CPLD_Ctrl_Reg             (*((volatile unsigned char *) 0x82010016))
#define CPLD_SRVON_Reg            (*((volatile unsigned char *) 0x82010018))

#define CPLD_PCx_Reg      (*((volatile unsigned char *) 0x8201001A))

#define CPLD_SVC_L_Reg      (*((volatile unsigned char *) 0x8201001C))
#define CPLD_SVC_H_Reg      (*((volatile unsigned char *) 0x8201001E))
#define CPLD_SVC_En_Reg     (*((volatile unsigned char *) 0x8201003E))


#define CPLD_Version_Reg          (*((volatile unsigned char *) 0x82010022))

//#define CPLD_XAXIS_H_Reg      (*((volatile unsigned char *) 0x82010002))
//

//Bit 4 Bit 5  PC+
#define CPLD_Port_Reg               (*((volatile unsigned char *) 0x82010024))

#define CPLD_HAHB_Reg            (*((volatile unsigned char *) 0x8201002A))
#define CPLD_PAS_PBS_L_Reg      (*((volatile unsigned char *) 0x82010026))
#define CPLD_PAS_PBS_H_Reg      (*((volatile unsigned char *) 0x82010028))


#define Bit_TEN0    (1)
#define Bit_TEN1    (1<<1)


#define Bit_ENWDALM (1)
#define Bit_EN8MS   (1<<2)
//-----------------
#define Bit_DOEN    (1)
#define Bit_SRVON   (1<<1)
#define Bit_PCDMR   (1<<7)

//-----------------------------------------------
//LPC2220 P0 Addr
#define LPC2220Port0Addr  ((volatile INT32U *)0xE0028000)
//LPC2220 P1 Addr
#define LPC2220Port1Addr  ((volatile INT32U *)0xE0028010)
//LPC2220 P2 Addr
#define LPC2220Port2Addr  ((volatile INT32U *)0xE0028020)
//
#define CPLD_Port_Reg_Addr               ((volatile INT32U *)0x82010024)