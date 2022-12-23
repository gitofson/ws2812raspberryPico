#ifndef __BUFFDRAW_H
#define __BUFFDRAW_H

#include <stdio.h>
#include <stdlib.h>


#define BD_BUFFER_HEIGHT           (14)
#define BD_BUFFER_WIDTH            (70)

#define BD_BUFFER_LENGTH           (BD_BUFFER_HEIGHT * BD_BUFFER_WIDTH)

void bdDrawBuffer(uint8_t *dst, const uint8_t *src);

#endif