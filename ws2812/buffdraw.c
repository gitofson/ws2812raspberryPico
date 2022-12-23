#include "buffdraw.h"
#include "string.h"
void bdDrawBuffer(uint8_t *dst, const uint8_t *src){
    uint16_t sIdx, sIdx2, dIdx;
    //uint8_t rowStartIdx=BD_BUFFER_HEIGHT/2-1;//6 for N_RSYMBOL_OWS=16
    int8_t rowBackwardIdx, rowForwardIdx;
    //for(rowForwardIdx=0, rowBackwardIdx=BD_BUFFER_HEIGHT/2-1; rowBackwardIdx >= 0; rowBackwardIdx-=2, rowForwardIdx+=2){
    for(rowForwardIdx=0, rowBackwardIdx=BD_BUFFER_HEIGHT/2-1; rowBackwardIdx >= 0; rowBackwardIdx-=2, rowForwardIdx+=2){
        ////for(i=0; i<=BD_BUFFER_HEIGHT/2*BD_BUFFER_WIDTH; i++){
            memcpy(dst + rowForwardIdx*BD_BUFFER_WIDTH, src + rowBackwardIdx*BD_BUFFER_WIDTH, BD_BUFFER_WIDTH);
            memcpy(dst + BD_BUFFER_HEIGHT/2*BD_BUFFER_WIDTH + rowForwardIdx*BD_BUFFER_WIDTH, src + BD_BUFFER_HEIGHT/2*BD_BUFFER_WIDTH + rowForwardIdx*BD_BUFFER_WIDTH, BD_BUFFER_WIDTH);

            if(rowBackwardIdx>0){
                for(dIdx=(rowForwardIdx+1)*BD_BUFFER_WIDTH, sIdx=rowBackwardIdx*BD_BUFFER_WIDTH-1, sIdx2=(BD_BUFFER_HEIGHT/2+rowForwardIdx+2)*BD_BUFFER_WIDTH-1; sIdx >= ((rowBackwardIdx-1)*BD_BUFFER_WIDTH); dIdx++, sIdx--, sIdx2--){
                    dst[dIdx]=src[sIdx];
                    dst[dIdx + BD_BUFFER_HEIGHT/2*BD_BUFFER_WIDTH] = src[sIdx2];
                }
            }
            
        ////}
    }
    //memcpy(dst->s+(BD_BUFFER_HEIGHT/2)*BD_BUFFER_WIDTH, &src[(BD_BUFFER_HEIGHT/2)*BD_BUFFER_WIDTH], BD_BUFFER_WIDTH );
}

void bdDrawBufferN(uint8_t *dst, const uint8_t *src){
    uint16_t sIdx, sIdx2, dIdx;
    int8_t rowBackwardIdx, rowForwardIdx;
    for(rowForwardIdx=0, rowBackwardIdx=BD_BUFFER_HEIGHT/2-1; rowBackwardIdx >= 0; rowBackwardIdx-=2, rowForwardIdx+=2){
        memcpy(dst + rowForwardIdx*BD_BUFFER_WIDTH, src + rowBackwardIdx*BD_BUFFER_WIDTH, BD_BUFFER_WIDTH);
        //memcpy(dst + (BD_BUFFER_HEIGHT-1)*BD_BUFFER_WIDTH - rowForwardIdx*BD_BUFFER_WIDTH, src + BD_BUFFER_HEIGHT/2*BD_BUFFER_WIDTH + rowForwardIdx*BD_BUFFER_WIDTH, BD_BUFFER_WIDTH);
        if(rowBackwardIdx>0){
            for(dIdx=(rowForwardIdx+1)*BD_BUFFER_WIDTH, sIdx=rowBackwardIdx*BD_BUFFER_WIDTH-1, sIdx2=(BD_BUFFER_HEIGHT/2+rowForwardIdx+2)*BD_BUFFER_WIDTH-1; sIdx >= ((rowBackwardIdx-1)*BD_BUFFER_WIDTH); dIdx++, sIdx--, sIdx2--){
                dst[dIdx]=src[sIdx];
                //dst[dIdx + (BD_BUFFER_HEIGHT-1)*BD_BUFFER_WIDTH - rowForwardIdx*BD_BUFFER_WIDTH] = src[sIdx2];
            }
        }
    }
}
