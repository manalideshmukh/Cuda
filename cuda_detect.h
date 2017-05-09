#ifndef _detect_header
#define _detect_header

#define R_min_B 9
#define R_max_B 23
#define G_min_B 13
#define G_max_B 26
#define B_min_B 24
#define B_max_B 38
#define R_min_R 86
#define R_max_R 107
#define G_min_R 9
#define G_max_R 35
#define B_min_R 24
#define B_max_R 51
#define R_min_W 155
#define R_max_W 208
#define G_min_W 151
#define G_max_W 182
#define B_min_W 146
#define B_max_W 159
#define R_min_Bl 44
#define R_max_Bl 86
#define G_min_Bl 48
#define G_max_Bl 78
#define B_min_Bl 108
#define B_max_Bl 145
#define R_min_Y 82
#define R_max_Y 87
#define G_min_Y 56
#define G_max_Y 59
#define B_min_Y 24
#define B_max_Y 35



void Image_Segment_Function(int, int);
void createTrackbars();
void MouseCallBackCropFunc(int, int, int, int, void*);
void MouseCallBackFunc(int, int, int, int, void*);
void onMouse(int, int, int, int, void*);

int flag = 0,H, S, V, R, G, B, R_MIN = 0, R_MAX = 255, G_MIN = 0, G_MAX = 255, B_MIN = 0, B_MAX = 255, rectcount = 0, ROIcount = 0, pos, R_high =0, R_low =0, G_high =0, G_low =0, B_high =0, B_low =0, flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0; 
int R_low1 =0 , R_high1 =0, G_low1 =0, G_high1 =0, B_low1 =0, B_high1 =0;



#endif 
