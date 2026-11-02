#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 将度分格式转换为十进制（如3413.67141 -> 34.227857）
static double nmea_to_decimal(double nmea_coord) {
    int degrees = (int)(nmea_coord / 100);
    double minutes = nmea_coord - degrees * 100;
    return degrees + minutes / 60.0;
}

// 校验NMEA语句的校验和（*后的十六进制值）
static int validate_checksum(const char* sentence) {
    const char* p = sentence + 1; // 跳过'$'
    char checksum = 0;
    while (*p && *p != '*') checksum ^= *p++;
    if (*p != '*') return 0; // 无校验和
    unsigned int expected = strtol(p + 1, NULL, 16);
    return (checksum == expected);
}

// 主解析函数
GPSData parse_gps_data(const char* gps_data) {
    GPSData data = {0};
    char* line = strtok((char*)gps_data, "\r\n"); // 按行分割

    while (line != NULL) {
        if (strncmp(line, "$GNRMC", 6) == 0 || strncmp(line, "$GPRMC", 6) == 0) {
            if (!validate_checksum(line)) continue;

            char* token = strtok(line, ",");
            int field = 0;
            while (token != NULL) {
                switch (field) {
                    case 1: strncpy(data.utc_time, token, sizeof(data.utc_time) - 1); break; // UTC时间
                    case 2: data.status = token[0]; break; // 定位状态
                    case 3: data.latitude = nmea_to_decimal(atof(token)); break; // 纬度
                    case 4: data.lat_direction = token[0]; break; // 纬度方向
                    case 5: data.longitude = nmea_to_decimal(atof(token)); break; // 经度
                    case 6: data.lon_direction = token[0]; break; // 经度方向
                    case 7: data.speed = atof(token) * 1.852; break; // 速度（节转km/h）
                    case 9: strncpy(data.utc_date, token, sizeof(data.utc_date) - 1); break; // UTC日期
                }
                token = strtok(NULL, ",");
                field++;
            }
        } 
        else if (strncmp(line, "$GNGGA", 6) == 0 || strncmp(line, "$GPGGA", 6) == 0) {
            if (!validate_checksum(line)) continue;

            char* token = strtok(line, ",");
            int field = 0;
            while (token != NULL) {
                switch (field) {
                    case 2: data.latitude = nmea_to_decimal(atof(token)); break; // 纬度
                    case 3: data.lat_direction = token[0]; break; // 纬度方向
                    case 4: data.longitude = nmea_to_decimal(atof(token)); break; // 经度
                    case 5: data.lon_direction = token[0]; break; // 经度方向
                    case 9: data.altitude = atof(token); break; // 海拔高度
                    case 7: data.satellites_used = atoi(token); break; // 使用卫星数
                }
                token = strtok(NULL, ",");
                field++;
            }
        }
        line = strtok(NULL, "\r\n");
    }
    return data;
}
