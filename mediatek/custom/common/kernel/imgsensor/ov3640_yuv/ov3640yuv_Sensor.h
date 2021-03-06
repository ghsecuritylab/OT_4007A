/* SENSOR FULL SIZE */
#ifndef __SENSOR_H
#define __SENSOR_H



typedef enum _OV3640_OP_TYPE_ {
        OV3640_MODE_NONE,
        OV3640_MODE_PREVIEW,
        OV3640_MODE_CAPTURE,
        OV3640_MODE_QCIF_VIDEO,
        OV3640_MODE_CIF_VIDEO,
        OV3640_MODE_QVGA_VIDEO
    } OV3640_OP_TYPE;

extern OV3640_OP_TYPE OV3640_g_iOV3640_Mode;

/* START GRAB PIXEL OFFSET */
#define IMAGE_SENSOR_START_GRAB_X		        2	// 0 or 1 recommended
#define IMAGE_SENSOR_START_GRAB_Y		        2	// 0 or 1 recommended

/* MAX/MIN FRAME RATE (FRAMES PER SEC.) */
#define MAX_FRAME_RATE							15		// Limitation for MPEG4 Encode Only
#define MIN_FRAME_RATE							12

/* SENSOR PIXEL/LINE NUMBERS IN ONE PERIOD */
    #define OV3640_FULL_PERIOD_PIXEL_NUMS  (2376)  // default pixel#(w/o dummy pixels) in UXGA mode
    #define OV3640_FULL_PERIOD_LINE_NUMS   (1568)  // default line#(w/o dummy lines) in UXGA mode
    #define OV3640_PV_PERIOD_PIXEL_NUMS    (OV3640_FULL_PERIOD_PIXEL_NUMS / 2)  // default pixel#(w/o dummy pixels) in SVGA mode
    #define OV3640_PV_PERIOD_LINE_NUMS     (784)   // default line#(w/o dummy lines) in SVGA mode

    /* SENSOR EXPOSURE LINE LIMITATION */
    #define OV3640_FULL_MAX_LINES_PER_FRAME    (1568)  // QXGA mode    
    #define OV3640_FULL_EXPOSURE_LIMITATION    (OV3640_FULL_MAX_LINES_PER_FRAME)
    #define OV3640_PV_MAX_LINES_PER_FRAME      (784)  // # of lines in one XGA frame    
    #define OV3640_PV_EXPOSURE_LIMITATION      (OV3640_PV_MAX_LINES_PER_FRAME)

/* SENSOR FULL SIZE */
   #define OV3640_IMAGE_SENSOR_FULL_WIDTH	   (2048-16)  
   #define OV3640_IMAGE_SENSOR_FULL_HEIGHT	 (1536-12)    



/* SENSOR PV SIZE */
#define OV3640_IMAGE_SENSOR_PV_WIDTH   (640-8)   
#define OV3640_IMAGE_SENSOR_PV_HEIGHT (480-6)


//SENSOR 3M size
#define OV3640_IMAGE_SENSOR_3M_WIDTH 	   (2048)	  
#define OV3640_IMAGE_SENSOR_3M_HEIGHT	   (1536)


#define OV3640_VIDEO_QCIF_WIDTH   (176)
#define OV3640_VIDEO_QCIF_HEIGHT  (144)

#define OV3640_VIDEO_30FPS_FRAME_LENGTH   (0x29E)
#define OV3640_VIDEO_20FPS_FRAME_LENGTH   (0x3ED)
#define OV3640_VIDEO_15FPS_FRAME_LENGTH   (0x53C)
#define OV3640_VIDEO_10FPS_FRAME_LENGTH   (0x7DA)

// SETUP TIME NEED TO BE INSERTED
#define OV3640_IMAGE_SENSOR_PV_INSERTED_PIXELS (390)
#define OV3640_IMAGE_SENSOR_PV_INSERTED_LINES  (9 - 6)

#define OV3640_IMAGE_SENSOR_FULL_INSERTED_PIXELS   (248)
#define OV3640_IMAGE_SENSOR_FULL_INSERTED_LINES    (11 - 2)

#define OV3640_PV_DUMMY_PIXELS			(0)
#define OV3640_VIDEO__CIF_DUMMY_PIXELS  (0)
#define OV3640_VIDEO__QCIF_DUMMY_PIXELS (0)

/* SENSOR SCALER FACTOR */
#define PV_SCALER_FACTOR					    3
#define FULL_SCALER_FACTOR					    1


/* DUMMY NEEDS TO BE INSERTED */
/* SETUP TIME NEED TO BE INSERTED */


/* SENSOR READ/WRITE ID */
	#define OV3640_WRITE_ID							    0x78
	#define OV3640_READ_ID								0x79



	/* SENSOR CHIP VERSION */
//	#define OV3640_SENSOR_ID    (0x364C)    // rev.2C




//s_add for porting
//s_add for porting
//s_add for porting

//export functions
UINT32 OV3640Open(void);
UINT32 OV3640GetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);
UINT32 OV3640GetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_INFO_STRUCT *pSensorInfo, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV3640Control(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow, MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);
UINT32 OV3640FeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId, UINT8 *pFeaturePara,UINT32 *pFeatureParaLen);
UINT32 OV3640Close(void);


//e_add for porting
//e_add for porting
//e_add for porting


#endif /* __SENSOR_H */
