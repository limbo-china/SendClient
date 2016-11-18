#ifndef _GETCONFIGFROMTEXT__H_
#define _GETCONFIGFROMTEXT__H_

#include <stdio.h>
#include <string.h>

#define LINE_MAX_LENGTH 256
#define NAME_MAX_LENGTH 50
#define VALUE_MAX_LENGTH 50 


int GetConfigValue(char *conf_path,char *conf_name,char *config_buff);

int GetAllConfig(char *conf_path,char *config_buff,int param_num);

#endif
