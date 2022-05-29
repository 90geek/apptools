#ifndef __APP_DEBUG_H__
#define __APP_DEBUG_H__

#define BLACK 	30
#define RED 	31
#define GREEN 	32
#define YELLOW 	33
#define BLUE 	34
#define COLOR2	35
#define COLOR3 	36
#define COLOR4 	37

#define DEBUG_LINE() 	printf("[%s:%s] line=%d\r\n",__FILE__, __func__, __LINE__)
#define MODULE_PRINT(color,fmt, module, args...) printf("\033[%dm[%s]"#fmt"\033[0m\r\n", color, module,  ##args)
#define MODULE_PRINT(color,fmt, module, args...) printf("\033[%dm[%s]"#fmt"\033[0m\r\n", color, module,  ##args)

#endif
