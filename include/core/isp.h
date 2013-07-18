#ifndef _ISP_H_
#define _ISP_H_

#define ISP_CTL		0x0001
#define ISP_CNC		0x0002
#define ISP_CM		0x0004
#define ISP_EDU		0x0008
#define ISP_HK		0x0010
#define ISP_TW		0x0020

//composite
#define ISP_CTL_CNC		0x0003
#define ISP_CTL_CM		0x0005
#define ISP_CNC_CM		0x0006
#define ISP_CTL_CNC_CM		0x0007

//fallbacks:
#define ISP_ALL			0xffff


#define AREA_BJ		0x0001
#define AREA_SH		0x0002
#define AREA_SZ		0x0003
#define AREA_CQ		0x0004
#define AREA_GZ		0x0005
#define AREA_NE		0x0006	
#define AREA_N		0x0007
#define AREA_NW		0x0008
#define AREA_ME		0x0009
#define AREA_M		0x000a
#define AREA_MW		0x000b
#define AREA_SE		0x000c
#define AREA_S		0x000d
#define AREA_SW		0x000e

#endif
