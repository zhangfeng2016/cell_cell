#include "stm32f10x.h"
#include "..\User\user_conf.h"
#include "..\SD_Card\sdio_sd.h"
#include "..\FATFS_V008A\src\ff.h"


FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc;         /* file objects */
FRESULT res;
UINT br;
FIL fsrc1;         /* file objects */

FIL config_file;
FRESULT config_file_res;
UINT config_file_br;

//int f_puts (const TCHAR*, FIL*);					/* Put a string to the file


uint8_t textFileBuffer[] = "hello world \r\n";
uint8_t textFileBuffer1[] = "+++++++++++++++++=================================++++++++++++\r\n ";
char  file_path[255] = "0000";
char  cofig_file_path[] = "0:/config.cfg";
uint8_t SD_Save_buf[512];
uint8_t Read_buf[512];
uint16_t sd_save_ptr = 0;
u8 Autosave_time;
u8 File_open_flag = 0;
#define FILE_OPEN  1
#define FILE_CLOSE  0

u8 sys_infer_1[9];
u8 sys_infer_2[9];
u8 sys_infer_3[9];
u8 sys_infer_4[9];
u8 sys_infer_5[9];
u8 sys_infer_6[9];
u8 sys_infer_7[9];
//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 chr2hex(u8 chr)
{
    if(chr >= '0' && chr <= '9')return chr - '0';
    if(chr >= 'A' && chr <= 'F')return (chr - 'A' + 10);
    if(chr >= 'a' && chr <= 'f')return (chr - 'a' + 10);
    return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 hex2chr(u8 hex)
{
    if(hex <= 9)return hex + '0';
    if(hex >= 10 && hex <= 15)return (hex - 10 + 'A');
    return '0';
}
/////////////////////////////////
//创建文件路径
//文件名已年月日为文件名格式为：20151111.txt
//
//
//
///////////////////////////////
void Creat_file_path(void)
{
    u16 year = timer.year;
    u8 month = timer.month;
    u8	day = timer.day;
    u8 i;

    char  path_buf[20];
    path_buf[0] = '0';
    path_buf[1] = ':';
    path_buf[2] = '/';
    if(year < 3000)
    {
        path_buf[3] = hex2chr(year / 1000);
        path_buf[4] = hex2chr((year % 1000) / 100);
        path_buf[5] = hex2chr((year % 100) / 10);
        path_buf[6] = hex2chr(year % 10);
    }
    path_buf[7] = hex2chr((month % 100) / 10);
    path_buf[8] = hex2chr(month % 10);
    path_buf[9] = hex2chr((day % 100) / 10);
    path_buf[10] = hex2chr(day % 10);
    path_buf[11] = '.';
    path_buf[12] = 't';
    path_buf[13] = 'x';
    path_buf[14] = 't';
    for (i = 0; i < 15; i++)
    {
        file_path[i] = path_buf[i];
    }
}
//////////////////////////////
//程序名称：void File_sys_int(void)
//初始化文件系统
//创建已日期为文件名的文件并打开文件
//
///
//
//
///
///////////////////////////////
void File_sys_int(void)
{
    Creat_file_path();
    f_mount(0, &fs);
    res = f_open( &fsrc , file_path , FA_CREATE_NEW | FA_WRITE);
    if ( res == FR_OK )
    {
        /* Write buffer to file */
        res = f_write(&fsrc, textFileBuffer1, sizeof(textFileBuffer1), &br);
    }
    else if ( res == FR_EXIST )
    {
           //res = f_lseek(&fsrc, fsrc.fsize);
           // f_puts (textFileBuffer1, &fsrc) ;  //?????????  
			res = f_write(&fsrc, textFileBuffer1, sizeof(textFileBuffer1), &br);
			 f_close(&fsrc);
    }
		//f_mount(1, &fs);
		config_file_res = f_open( &config_file , cofig_file_path , FA_CREATE_NEW | FA_WRITE);
    if ( config_file_res == FR_OK )
    {
        /* Write buffer to file */
        config_file_res = f_write(&config_file, textFileBuffer1, sizeof(textFileBuffer1), &br);
    }
    else if ( config_file_res == FR_EXIST )
    {
             config_file_res = f_lseek(&config_file, config_file.fsize);
            f_puts (textFileBuffer1, &fsrc) ;  //?????????  
			
    }		
}
//////////////////////////////
//函数名称：void str_change_buf(void)
//准备要存储的数据
//将数据转化为字符碵??]格式
//
///
//
//
///
///////////////////////////////
void str_push_to_buf(uint8 *str)
{
    u8 temp_i, temp_j;
    temp_i = sizeof(str);
    if((sd_save_ptr + temp_i) < 512) //防止BUF溢出
    {
        for(temp_j = 0; temp_j < temp_i; temp_j++)
        {
            SD_Save_buf [ sd_save_ptr++] = str[temp_j];
        }
    }
}
//////////////////////////////
//函数名称：void str_change_buf(void)
//准备要存储的数据
//将数据转化为字符碵??]格式
//
///
//
//
///
///////////////////////////////
void num_push_to_buf(uint32 num)
{
    u8 temp_i;//temp_j;
    //temp_i=sizeof(str);
    if((sd_save_ptr + temp_i) < 512) //防止BUF溢出
    {
        SD_Save_buf [ sd_save_ptr++] = ';';
        if(num > 0xffff)
        {
            //SD_Save_buf [ sd_save_ptr++] =hex2chr((num>>24)&0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 28) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 24) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 20) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 16) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 12) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 8) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 4) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr(num & 0x00F);
        }
        else
        {
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 12) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 8) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr((num >> 4) & 0x00F);
            SD_Save_buf [ sd_save_ptr++] = hex2chr(num & 0x00F);

        }

    }
}
//////////////////////////////
//函数名称：void str_change_buf(void)
//准备要存储的数据
//将数据转化为字符碵??]格式
//
///
//
//
///
///////////////////////////////
void Time_push_to_buf(void )
{
    u8 hour, min, sec;
    hour = timer.hour;
    min = timer.min;
    sec = timer.sec;
    SD_Save_buf [ sd_save_ptr++] = hex2chr((hour % 100) / 10);
    SD_Save_buf [ sd_save_ptr++] = hex2chr(hour % 10);
    SD_Save_buf [ sd_save_ptr++] = ':';
    SD_Save_buf [ sd_save_ptr++] = hex2chr((min % 100) / 10);
    SD_Save_buf [ sd_save_ptr++] = hex2chr(min % 10);
    SD_Save_buf [ sd_save_ptr++] = ':';
    SD_Save_buf [ sd_save_ptr++] = hex2chr((sec % 100) / 10);
    SD_Save_buf [ sd_save_ptr++] = hex2chr(sec % 10);
}
//////////////////////////////
//函数名称：void Chage_hex_to_str(void)
//准备要存储的数据
//将数据转化为字符碵??]格式
//
///对于打开的文件每30s自动保存一次
//
//
///
///////////////////////////////
void Change_hex_to_str(void)
{
    u8 temp_i, temp_j;
    u8 temp_k;
    u8 hour, min, sec;

    if(1)//有数据需要保存
    {
        Time_push_to_buf();
        str_push_to_buf(" ad ");
        num_push_to_buf(Monitor_Air_AD);		 
        num_push_to_buf(Monitor_Green_AD);		  
        num_push_to_buf(Monitor_Red_AD);		 
        num_push_to_buf(Monitor_Blood_AD);
        num_push_to_buf(Monitor_Reservoir_weigh);
        str_push_to_buf("STATE ");
        num_push_to_buf(Mode);
        num_push_to_buf(work_sta);
        num_push_to_buf(Pump_Sta);
        str_push_to_buf("FLOW");
        num_push_to_buf(Monitor_Pre_fill_Volume);
        num_push_to_buf(Monitor_Fill_Volume);
        num_push_to_buf(Monitor_Wash_Volume);
        num_push_to_buf(Monitor_Empty_Volume);
        str_push_to_buf("ALARM");
        num_push_to_buf(Alarm_sta);
			  str_push_to_buf("KEY; ");
        if(Key_save_flag == 1)
        {
            Key_save_flag = 0;
            num_push_to_buf(Key_num);
					  SD_Save_buf[sd_save_ptr++] = '\r';
            SD_Save_buf[sd_save_ptr++] = '\n';
            Key_num = 0;
        }
				else
				{

        SD_Save_buf[sd_save_ptr++] = '\r';
        SD_Save_buf[sd_save_ptr++] = '\n';
					SD_Save_buf[sd_save_ptr++] = 0;
        sd_save_ptr = 0;
				}
    }
}
//////////////////////////////
//函数名称：void Prepare_buf_to_save(void)
//准备要存储的数据
//将数据转化为字符碵??]格式
//
///对于打开的文件每30s自动保存一次 
//
//
///
///////////////////////////////
void Prepare_buf_to_save(void)
{
    SD_Save_buf[0] = 1;
}

//////////////////////////////
//函数名称：void Write_data_to_SD(void)
//向SD卡中写入数据
//创建已日期为文件名的文件并打开文件
//
///对于打开的文件每30s自动保存一次
//
//
///
///////////////////////////////
void Write_data_to_SD(void)
{
    Change_hex_to_str();
    if(1)// have data to save
    {
        if(File_open_flag == FILE_OPEN) //file open
        {
            res = f_lseek(&fsrc, fsrc.fsize);
            f_puts (SD_Save_buf, &fsrc) ;  //?????????
            Autosave_time++;
            if(Autosave_time > 5)
            {
                f_close(&fsrc);
                Autosave_time = 0;
                File_open_flag = FILE_CLOSE;
            }

        }
        else//file close
        {
            res = f_open( &fsrc , file_path , FA_OPEN_ALWAYS | FA_WRITE);
            res = f_lseek(&fsrc, fsrc.fsize);
            f_puts (SD_Save_buf, &fsrc) ;  //?????????
            File_open_flag = FILE_OPEN;
        }
//			br=3;
//			f_open (&fsrc,file_path,FA_READ); 
//			f_lseek(&fsrc,6);
//			f_read(&fsrc,Read_buf,2,&br);
//			f_lseek(&fsrc,6+br);
//			f_read(&fsrc,Read_buf+br,2,&br);

      			
    }
}
