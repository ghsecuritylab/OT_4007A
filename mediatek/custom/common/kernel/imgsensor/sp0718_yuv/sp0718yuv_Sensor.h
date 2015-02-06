
 
#ifndef __SP0718_SENSOR_H
#define __SP0718_SENSOR_H


#define VGA_PERIOD_PIXEL_NUMS						784//694
#define VGA_PERIOD_LINE_NUMS						510//488

#define IMAGE_SENSOR_VGA_GRAB_PIXELS			0
#define IMAGE_SENSOR_VGA_GRAB_LINES			1

#define IMAGE_SENSOR_VGA_WIDTH					(640)
#define IMAGE_SENSOR_VGA_HEIGHT					(480)

#define IMAGE_SENSOR_PV_WIDTH					(IMAGE_SENSOR_VGA_WIDTH - 8)
#define IMAGE_SENSOR_PV_HEIGHT					(IMAGE_SENSOR_VGA_HEIGHT - 6)

#define IMAGE_SENSOR_FULL_WIDTH					(IMAGE_SENSOR_VGA_WIDTH - 8)
#define IMAGE_SENSOR_FULL_HEIGHT					(IMAGE_SENSOR_VGA_HEIGHT - 6)

#define SP0718_WRITE_ID							0x42
#define SP0718_READ_ID								0x43
#define SP0718_SENSOR_ID                                              0x71
typedef enum
{
	SP0718_RGB_Gamma_m1 = 0,
	SP0718_RGB_Gamma_m2,
	SP0718_RGB_Gamma_m3,
	SP0718_RGB_Gamma_m4,
	SP0718_RGB_Gamma_m5,
	SP0718_RGB_Gamma_m6,
	SP0718_RGB_Gamma_night
}SP0718_GAMMA_TAG;

typedef enum
{
	CHT_806C_2 = 1,
	CHT_808C_2,
	LY_982A_H114,
	XY_046A,
	XY_0620,
	XY_078V,
	YG1001A_F
}SP0718_LENS_TAG;

UINT32 SP0718_Open(void);
UINT32 SP0718_Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 SP0718_FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 SP0718_GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 SP0718_GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 SP0718_Close(void);

#endif /* __SENSOR_H */

