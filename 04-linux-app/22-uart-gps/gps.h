#ifndef __GPS_H__
#define __GPS_H__

typedef struct {
    double latitude;      // 十进制纬度（如34.227857）
    double longitude;     // 十进制经度（如108.880005）
    char lat_direction;   // 纬度方向（N/S）
    char lon_direction;   // 经度方向（E/W）
    char status;          // 定位状态（A=有效，V=无效）
    float altitude;       // 海拔高度（米）
    float speed;          // 地面速度（km/h）
    int satellites_used;  // 使用卫星数量
    char utc_time[12];    // UTC时间（HHMMSS.sss）
    char utc_date[12];    // UTC日期（DDMMYY）
} GPSData;

extern GPSData parse_gps_data(const char* gps_data);

#endif