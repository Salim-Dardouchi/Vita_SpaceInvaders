/*
 * app.h
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

typedef struct s_app t_app;

t_app*AppNew(char*appTitleStr, int iWidth, int iHeight);
t_app*AppDel(t_app*pApp);
int AppRun(t_app*pApp);
