#include "device.h"
#include "dev_t.h"
#include "device_register.h"
#include "device_attrs.h" 
#include "error_report.h" 
#include <string.h>
#include <malloc.h>
#include <stdio.h>

static const char* file = "device_register.c";


/**
 * 输入：无
 * 输出：无
 * 功能：注册设备结构体
 */
int register_devices(void)
{
       
       devno_t devno;
       char *lid, *type, *interface;
       void* attrs_item;
       unsigned int attrs_item_num;
       const char* func = "register_devices";       
       
       int i;
       attrs_item_num = get_attrs_item_num();
       for (i=0; i<attrs_item_num; i++){
           //获取设备的属性
          attrs_item = get_device_attrs_item();
          lid = get_device_lid(attrs_item);
          type = get_device_type(attrs_item);
          interface = get_device_interface(attrs_item);

          //根据设备类型申请设备号
          devno = alloc_devno(type);

          //分配并初始化设备结构体
          struct device* devp = (struct device*)malloc(sizeof(struct device));
          if(!check_null(file, func, "devp", devp)) return FAILURE;

          devp->devno = devno;
          strcpy(devp->interface, interface);
          strcpy(devp->lid, lid);
          devp->device_operation = NULL;
          devp->private_data = NULL;

          //建立设备逻辑标识符与设备结构体的连接
          //lid2dev_mapping(lid, devp);
           lid2dev_mapping(devp->lid, devp);
        
          //注册到设备索引表中
          add_device(GET_MAJOR(devno), devp);
       }

        return SUCCESS;
}




