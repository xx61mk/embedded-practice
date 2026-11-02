#include "uart.h"
#include "gps.h"

int main(void)
{
    int uartfd = 0;
    int nret = 0;
    char tmpbuff[4096] = {0};
    GPSData parsed; 

    uartfd = open_serial_port("/dev/ttymxc2");
    if (-1 == uartfd)
    {
        printf("open_serial_port failed\n");
        return -1;
    }

    while (1)
    {   
        memset(tmpbuff, 0, sizeof(tmpbuff));
        nret = read_from_serial(uartfd, tmpbuff, sizeof(tmpbuff));
        
        memset(&parsed, 0, sizeof(parsed));
        parsed = parse_gps_data(tmpbuff);

        printf("纬度: %.6f°%c\n", parsed.latitude, parsed.lat_direction);
        printf("经度: %.6f°%c\n", parsed.longitude, parsed.lon_direction);
        printf("状态: %c\n", parsed.status);
        printf("海拔: %.1f米\n", parsed.altitude);
        printf("速度: %.2f km/h\n", parsed.speed);
        printf("卫星数: %d\n", parsed.satellites_used);

        sleep(5);
    }
    

    close_serial_port(uartfd);

    return 0;
}