#include "stdio.h"
#include "r245.h"

int main(void)
{
    
    unsigned long ft_status;
    unsigned long num_devs;
    void * ft_handle;
    short int dev_number = 0, dev_addr = 1;
    unsigned char data[1];
    unsigned char version[50];

    ft_status = FT_CreateDeviceInfoList(&num_devs);
    if (ft_status == FT_OK) {
      printf("Number of devices is %d\n", num_devs);
    }
    else {
        printf("Error: create device info list\n");
    }

    if(num_devs > 0)
    {
        ft_status = R245_InitDev(dev_number, &ft_handle);
        if(R245_GetVersion(ft_handle, dev_addr, version) == FT_OK)
        {
            printf("Version = %s\n", version);
        } else
        {
            printf("Error: get version\n");
        }

        ft_status = R245_AuditEn(ft_handle, dev_addr, 1);

        if(ft_status == R245_OK)
        {
            printf("Trans is OK");
        }

        FT_Close(ft_handle);
    }

    return 0;
}
