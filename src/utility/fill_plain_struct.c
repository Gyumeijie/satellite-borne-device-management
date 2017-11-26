#include "config_info_collect.h"
#include "error_report.h"
#include "config_info_collect.h"
#include "fill_plain_struct.h"
#include "miscellaneous.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>


/**
 *　输入：操作名op_name，模板参数para_list的名字para_name，包含驱动模板中待收集
 *        的类型为struct的模板参数的每个成员信息结构体数组st以及执行具体填充工作
 *        的函数do_fill
 *　输出：填充类型为struct的模板参数是否成功
 *　功能：为给定操作名填充类型为struct的模板参数
 */
int fill_plain_struct
(
   const char* dts_owner_name, 
   const char* para_list_name, 
   struct struct_member st[], 
   struct_fill_func_ptr do_fill
)
{
   void* para_list = find_para_list(dts_owner_name, para_list_name);
   if (para_list == NULL)  return UNMATCH;

   int num_para = get_para_list_length(para_list);
   if (num_para == -1)  return UNMATCH;

   void* first_para = get_first_para(para_list);

   return do_fill_plain_struct(first_para, num_para, st, do_fill);
}


static int do_fill_plain_struct
(
   const void* first_para,
   int num_para,
   struct struct_member st[],
   struct_fill_func_ptr do_fill
)
{

   int i;
   const void* para = first_para;
   for (i=0; i<num_para; i++){
       // 检查是否有对应的结构体成员，如果没有则表示不匹配
       const char* name = st[i].name;
       if (! is_equal(name, get_element_data(para, "name"))) return FAILURE;

       // 检查type是否一致，如果不是则表示不匹配
       if (! check_para_data_type(para, name, st[i].type)) return FAILURE;

       // 将成员的数据写入到相应的结构体中
       const char* value_str = get_element_data(para, "text_value");
       void* data = string_to_numeric_value(value_str, st[i].type);
       do_fill(st[i].index, data);
       
       // 获取下一个para项
       para = get_next_sibling(para);
   }

    return SUCCESS;
}
