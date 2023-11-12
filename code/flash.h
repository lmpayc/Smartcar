#ifndef _FLASH_H_
#define _FLASH_H_

#include "all_define.h"
#include "zf_common_headfile.h"


void key_judge();
void save_int (int p,int r);
void page_show(int page);
void show_int(int k,int high);
int mod_change(int *flag);
void row_show(int row);
void page_change(int *page);
void try_data();
void change_int(int page,int row);
void init_data();


#endif
