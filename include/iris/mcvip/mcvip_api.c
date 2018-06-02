/*
 * mcvip_api.h
 *
 * TVP5158 decoder previlege mode defined.
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <mcvip_priv.h>
#include <audio_dac_priv.h>
//#include <drv_gpio.h>

#include <drv_audio.h>
MCVIP_Hndl  gMCVIP_hndl[MCVIP_VIDEO_INPUT_PORT_MAX];
MCVIP_Ctrl  gMCVIP_ctrl;

int MCVIP_init(int devAddr[], int audioDacAddr)
{
    int status, i = 0, j = 0, idx = 0;

    memset( gMCVIP_hndl , 0 , sizeof( gMCVIP_hndl ) );
    memset( &gMCVIP_ctrl , 0 , sizeof( gMCVIP_ctrl ) );

    status = OSA_mutexCreate(&gMCVIP_ctrl.dmaLock);
    OSA_assertSuccess( status );

    status = DRV_dmaOpen( &gMCVIP_ctrl.dmaMemcpyChHndl , DRV_DMA_MODE_NORMAL , 2 );

    if(status!=OSA_SOK)
    {
        OSA_ERROR("DMA channel allocation failed\n");
        return OSA_EFAIL;
    }

    for(j = 0; j < MCVIP_VIDEO_INPUT_PORT_MAX; j++)
    {
        for(i = 0; i < MCVIP_TVP5158_MAX_CASCADE; i++)
        {
            gMCVIP_ctrl.i2cHndl[j][i].fd = -1;
            if( devAddr[idx] > 0 )
            {
                status = DRV_i2cOpen(&gMCVIP_ctrl.i2cHndl[j][i], devAddr[idx]);

                if(status!=OSA_SOK)
                {
                    OSA_ERROR("DRV_i2cOpen(0x%x)\n", devAddr[idx]);
                    goto error_exit;
                }

                status = TVP5158_checkChipId(&gMCVIP_ctrl.i2cHndl[j][i]);

                if(status!=OSA_SOK)
                {
                    OSA_ERROR("TVP5158_checkChipId(0x%x)\n", devAddr[idx]);
                    goto error_exit;
                }
            }
            idx++;
        }
    }

    status = DRV_dmaOpen(&gMCVIP_ctrl.dmaMemcpyChHndl, DRV_DMA_MODE_NORMAL, 2);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("DMA channel allocation failed\n");
        return OSA_EFAIL;
    }

    error_exit:
    if(status!=OSA_SOK)
    {
        DRV_dmaClose(&gMCVIP_ctrl.dmaMemcpyChHndl);
        DRV_dmaExit();
        OSA_mutexDelete(&gMCVIP_ctrl.dmaLock);

        for(j=0; j<MCVIP_VIDEO_INPUT_PORT_MAX; j++)
        {
            for(i=0; i<MCVIP_TVP5158_MAX_CASCADE; i++)
            {
                if(gMCVIP_ctrl.i2cHndl[j][i].fd >= 0)
                {
                    DRV_i2cClose(&gMCVIP_ctrl.i2cHndl[j][i]);
                }
            }
        }
    }

    return status;
}

int MCVIP_exit()
{
    int i, j;

    DRV_dmaClose(&gMCVIP_ctrl.dmaMemcpyChHndl);

    OSA_mutexDelete(&gMCVIP_ctrl.dmaLock);

    for(j=0; j<MCVIP_VIDEO_INPUT_PORT_MAX; j++)
    {
        for(i=0; i<MCVIP_TVP5158_MAX_CASCADE; i++)
        {
            if(gMCVIP_ctrl.i2cHndl[j][i].fd >= 0)
	    {
                DRV_i2cClose(&gMCVIP_ctrl.i2cHndl[j][i]);
	    }
        }
    }

    return OSA_SOK;
}

int MCVIP_sendCmd(MCVIP_Hndl *hndl, Uint16 cmd, void *prm, Uint16 flags)
{
    return OSA_mbxSendMsg(&hndl->tskHndl.mbxHndl, &hndl->mbxHndl, cmd, prm, flags);
}

void *MCVIP_create(MCVIP_CreatePrm *prm)
{
    MCVIP_Hndl *pHndl;
    int status;
    
    if( prm==NULL )
    {
        return NULL;
    }
    
    if( prm->videoInputPort >= MCVIP_VIDEO_INPUT_PORT_MAX )
    {
        return NULL;
    }
    
    pHndl = &gMCVIP_hndl[prm->videoInputPort];

    pHndl->state=MCVIP_STATE_NOT_CREATED;

    memcpy( &pHndl->createPrm, prm, sizeof(pHndl->createPrm));

    status = OSA_tskCreate( &pHndl->tskHndl, MCVIP_tskMain, prm->captureThrPri, MCVIP_STACK_SIZE, 0);

    OSA_assertSuccess(status);
    
    status = OSA_mbxCreate( &pHndl->mbxHndl);

    OSA_assertSuccess(status);

    status = MCVIP_sendCmd(pHndl, MCVIP_CMD_CREATE, pHndl, OSA_MBX_WAIT_ACK );

    if(status!=OSA_SOK)
    {
        OSA_tskDelete(&pHndl->tskHndl);
        OSA_mbxDelete(&pHndl->mbxHndl);
        return NULL;
    }
    
    pHndl->state=MCVIP_STATE_CREATED;

    return pHndl;
}

int MCVIP_delete(void *hndl)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    int status;
    
    if(pHndl==NULL)
        return OSA_EFAIL;

    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return OSA_EFAIL;

    status = MCVIP_sendCmd(pHndl, MCVIP_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK );

    status = OSA_tskDelete( &pHndl->tskHndl );

    OSA_assertSuccess(status);
    
    status = OSA_mbxDelete( &pHndl->mbxHndl);

    OSA_assertSuccess(status);

    pHndl->state=MCVIP_STATE_NOT_CREATED;

    return OSA_SOK;
}

int MCVIP_getChList(void *hndl, MCVIP_ChList *chInfo)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl==NULL)
        return OSA_EFAIL;

    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return OSA_EFAIL;

    memcpy(chInfo, &pHndl->chList, sizeof(pHndl->chList));

    return OSA_SOK;
}

int MCVIP_start(void *hndl)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    if(pHndl==NULL) return OSA_EFAIL;
    if(pHndl->state==MCVIP_STATE_NOT_CREATED) return OSA_EFAIL;
    return MCVIP_sendCmd(pHndl, MCVIP_CMD_START, NULL, OSA_MBX_WAIT_ACK);
}

int MCVIP_forceWakeup(void *hndl)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    if(pHndl==NULL) return OSA_EFAIL;
    return OSA_quePut(&pHndl->fullQue, 0xFF, OSA_TIMEOUT_FOREVER);
}

int MCVIP_stop(void *hndl)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    int status, i;

    if(pHndl==NULL)
    {
        return OSA_EFAIL;
    }

    if( pHndl->state == MCVIP_STATE_NOT_CREATED )
    {
        return OSA_EFAIL;
    }

    //    OSA_printf( "pHndl->emptyQue[i], 0xFF\n" );
    //    for(i=0; i<pHndl->chList.numCh; i++)
    //    {
    //        status = OSA_quePut(&pHndl->emptyQue[i], 0xFF, OSA_TIMEOUT_FOREVER);
    //    }

    OSA_printf( "MCVIP_CMD_STOP, 0xFF 1111111111111\n" );
    status = MCVIP_sendCmd( pHndl, MCVIP_CMD_STOP, NULL, OSA_MBX_WAIT_ACK ); //OSA_MBX_FREE_PRM
    OSA_printf( "MCVIP_CMD_STOP, 0xFF 22222222222\n" );

    return status;
}

int MCVIP_getBuf(void *hndl, int *id, unsigned int timeout)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    int status;

    if(pHndl==NULL)
        return OSA_EFAIL;

    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return OSA_EFAIL;

    *id = -1;
    status = OSA_queGet( &pHndl->fullQue , id , timeout);
    if(status==OSA_SOK)
    {
        if( *id>=0 && *id < pHndl->createPrm.numBuf )
        {
            status = OSA_SOK;
        }
        else
        {
            OSA_printf( "MCVIP_getBuf OSA_EFAIL  !!!!\n" );
            status = OSA_EFAIL;
        }
    }
    return status;
}

int MCVIP_putBuf(void *hndl, int id, int flags)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;
    int status=OSA_SOK;
    Bool releaseBuf=FALSE;

    if(pHndl==NULL)
        return OSA_EFAIL;

    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return OSA_EFAIL;

    if(id<0 || id>=pHndl->createPrm.numBuf)
        return OSA_EFAIL;
    
    OSA_mutexLock(&pHndl->putBufLock);

    pHndl->bufInfo[id].flags |= (flags & MCVIP_FLAG_ALL_DONE );

    if( (pHndl->bufInfo[id].flags & MCVIP_FLAG_ALL_DONE) == MCVIP_FLAG_ALL_DONE )
    {
        pHndl->bufInfo[id].flags = 0;
        releaseBuf = TRUE;
    }

    OSA_mutexUnlock(&pHndl->putBufLock);

    if( releaseBuf )
    {
        status = OSA_quePut(&pHndl->emptyQue[pHndl->bufInfo[id].chId], id , OSA_TIMEOUT_FOREVER);
    }

    return status;
}

MCVIP_BufInfo *MCVIP_getBufInfo(void *hndl, int id)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl==NULL)
        return NULL;

#if 0
    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return NULL;
#endif

    if(id<0 || id>=pHndl->createPrm.numBuf)
        return NULL;

    return &pHndl->bufInfo[id];
}

int MCVIP_getNumCh(int videoDecoderMode)
{
    switch(videoDecoderMode) {
    case MCVIP_VIDEO_DECODER_MODE_2CH_D1:        
    case MCVIP_VIDEO_DECODER_MODE_2CH_D1_CROP:        
        return 2;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF:            
    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_CROP:            
    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1:
    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_CROP:
        return 4;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1:
    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP:
    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1:    
    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP:    
        return 5;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF:
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_CROP:
        return 8;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1:        
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP:        
        return 9;

    default:
        return 0;
    }

    return 0;
}

int MCVIP_saveFrame(void *hndl)
{
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl==NULL)
        return OSA_EFAIL;

    if(pHndl->state==MCVIP_STATE_NOT_CREATED)
        return OSA_EFAIL;

    pHndl->saveFrame = TRUE;

    return OSA_SOK;
}

int MCVIP_getChInfo(int videoDecoderMode, int videoSystem, MCVIP_ChInfo *info)
{
    int width, height;

    switch(videoDecoderMode) {
    case MCVIP_VIDEO_DECODER_MODE_2CH_D1:          
        width = 720;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 480;
        else
            height = 576;
        break;

    case MCVIP_VIDEO_DECODER_MODE_2CH_D1_CROP:          
        width = 704;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 480;
        else
            height = 576;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1:            
    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1:
        width = 360;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 480;
        else
            height = 576;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_CROP:            
    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP:
        width = 352;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 480;
        else
            height = 576;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF:            
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF:
    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1:
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1:            
        width = 360;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 240;
        else
            height = 288;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_CROP:            
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_CROP:
    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP:
    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP:            
        width = 352;
        if(videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            height = 240;
        else
            height = 288;
        break;

    default:
        width=height=0;
        break;
    }

    info->width   = width;
    info->height  = height;
    //  info->offsetH = OSA_align(width, 32); //MAT - DM365
    info->offsetH = OSA_align(width, 16); //MAT - DM6446
    info->offsetV = height;

    if(videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1      ||
       videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1      ||
       videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1  ||
       videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP ||
       videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP ||
       videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP )
    {
        //  info->offsetH = OSA_align(720, 32); //MAT - DM365
        info->offsetH = OSA_align(720, 16); //MAT - DM6446

        if(videoSystem == MCVIP_VIDEO_SYSTEM_NTSC)
        {
            info->offsetV = 480;
        }
        else
        {
            info->offsetV = 576;
        }
    }

    return OSA_SOK;
}

int MCVIP_getBufSize(int videoDecoderMode, int videoSystem)
{
    MCVIP_ChInfo info;

    MCVIP_getChInfo(videoDecoderMode, videoSystem, &info);

    return info.offsetH*info.offsetV*2;
}

int MCVIP_dmaRun(MCVIP_DmaPrm *prm)
{
    DRV_DmaCopy2D copy2D[2];
    DRV_DmaFill2D fill2D[2];
    unsigned long physAddr;
    int status;

    if(prm->dstVirtAddr==NULL || prm->copyWidth<=0 || prm->copyHeight<=0 || prm->dstOffsetH<=0 || prm->dstOffsetV<=0)
    {
        OSA_ERROR("Invalid parameters (%d, %d, %d, %d, %d)\n", (Uint32)prm->dstVirtAddr, prm->copyWidth, prm->copyHeight, prm->dstOffsetH, prm->dstOffsetV);
        return OSA_EFAIL;
    }
    
    if(prm->srcVirtAddr==NULL)
    {
        // do fill
        physAddr = (unsigned long)prm->dstPhysAddr;
        if(physAddr==(unsigned long)NULL)
        {
            physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->dstVirtAddr);
        }

        fill2D[0].dstVirtAddr = (unsigned long)prm->dstVirtAddr;
        fill2D[0].dstPhysAddr = physAddr;
        fill2D[0].copyWidth = prm->copyWidth*2;
        fill2D[0].copyHeight = prm->copyHeight;
        fill2D[0].dstOffsetH = prm->dstOffsetH*2;
        fill2D[0].fillValue = prm->fillValueY;

        OSA_mutexLock(&gMCVIP_ctrl.dmaLock);

        status = DRV_dmaFill2D(&gMCVIP_ctrl.dmaMemcpyChHndl, fill2D, 1);

        OSA_mutexUnlock(&gMCVIP_ctrl.dmaLock);
    }
    else
    {

        if(prm->srcOffsetH<=0 || prm->srcOffsetV<=0)
        {
            return OSA_EFAIL;
        }

        physAddr = (unsigned long)prm->dstPhysAddr;
        if(physAddr==(unsigned long)NULL)
        {
            physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->dstVirtAddr);
        }

        copy2D[0].dstPhysAddr = physAddr;

        physAddr = (unsigned long)prm->srcPhysAddr;
        if(physAddr==(unsigned long)NULL)
        {
            physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->srcVirtAddr);
        }

        copy2D[0].srcPhysAddr = physAddr;
        copy2D[0].copyWidth   = prm->copyWidth*2;
        copy2D[0].copyHeight  = prm->copyHeight;
        copy2D[0].dstOffsetH  = prm->dstOffsetH*2;
        copy2D[0].srcOffsetH  = prm->srcOffsetH*2;
        copy2D[0].skipH       = 0;

        OSA_mutexLock(&gMCVIP_ctrl.dmaLock);

        status = DRV_dmaCopy2D(&gMCVIP_ctrl.dmaMemcpyChHndl, copy2D, 1);

        OSA_mutexUnlock(&gMCVIP_ctrl.dmaLock);
    }

    if(status!=OSA_SOK)
    {
        OSA_ERROR("DRV_dmaCopyFill2D() (chId = %d)\n", gMCVIP_ctrl.dmaMemcpyChHndl.chId);
    }

    return status;
}

int MCVIP_getTVP5158Status(void *hndl, int *chLockStatus)
{
    int status;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl==NULL)
        return OSA_EFAIL;
    
    status = MCVIP_tvp5158GetChLockStatus(pHndl);

    if(status == OSA_SOK)
	memcpy(chLockStatus, pHndl->chLockStatus, sizeof(pHndl->chLockStatus));

    return status;
}

// AYK - 0826
// AUDIO APIs

// audio init
int MCVIP_audioInit(void *hndl,MCVIP_CreatePrm *prm)
{
    int ret;
    DRV_AudioConfig audioConfig;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
    {
        return OSA_EFAIL;
    }

    pHndl->createPrm.format         = prm->format;
    pHndl->createPrm.numChannels    = prm->numChannels;
    pHndl->createPrm.samplingRate   = prm->samplingRate;
    pHndl->createPrm.enablePlayback = prm->enablePlayback;

    // Open the audio capture device
    audioConfig.format       = pHndl->createPrm.format;
    audioConfig.numChannels  = pHndl->createPrm.numChannels;
    audioConfig.samplingRate = pHndl->createPrm.samplingRate;

    ret = DRV_audioOpenRec(&pHndl->audioHndl,&audioConfig);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    if(pHndl->createPrm.enablePlayback)
    {
        // open audio playback device
        ret = DRV_audioOpenPlay(&pHndl->audioHndl,&audioConfig);

        if(ret < 0)
        {
            return OSA_EFAIL;
        }
    }
    return OSA_SOK;
}

// audio record
int MCVIP_audioRecord(void *hndl,char *recBuf,int recBufSize)
{
    int ret;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
    {
        return OSA_EFAIL;
    }

    // Read from the capture device
    ret = DRV_audioRecord(&pHndl->audioHndl,recBuf,recBufSize);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

// audio playback
int MCVIP_audioPlayback(void *hndl,char *playBuf,int playBufSize)
{
    int ret;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
    {
        return OSA_EFAIL;
    }

    if(pHndl->createPrm.enablePlayback == 0)
    {
        return OSA_EFAIL;
    }

    // write to the playback device
    ret = DRV_audioPlay(&pHndl->audioHndl,playBuf,playBufSize);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

// set volume
int MCVIP_audioSetVol(void *hndl,Uint8 channel,Uint8 vol)
{
    int ret;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
    {
        return OSA_EFAIL;
    }

    pHndl->createPrm.audVol[channel] = vol;

    ret = MCVIP_tvp5158SetAudVol(pHndl,channel);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

// set Mute
int	MCVIP_audioSetMute(void *hndl,Uint8 channel,Uint8 enableMute)
{
    int ret;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
        return OSA_EFAIL;

    pHndl->createPrm.audMute[channel] = enableMute;

    ret = MCVIP_tvp5158SetAudMute(pHndl,channel);

    if(ret < 0)
        return OSA_EFAIL;

    return OSA_SOK;
}

// audio exit
int MCVIP_audioExit(void *hndl)
{
    int ret;
    MCVIP_Hndl *pHndl = (MCVIP_Hndl *)hndl;

    if(pHndl == NULL)
    {
        return OSA_EFAIL;
    }

    // close audio capture device
    ret = DRV_audioCloseRec(&pHndl->audioHndl);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    if(pHndl->createPrm.enablePlayback)
    {
        // close audio playback device
        ret = DRV_audioClosePlay(&pHndl->audioHndl);

        if(ret < 0)
        {
            return OSA_EFAIL;
        }
    }

    return OSA_SOK;
}

