/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbfuncother.c,v 1.8 2006/12/07 22:10:34 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"

void SetInfo(UCHAR *Frame, USHORT *Len );



/* ----------------------- Static variables ---------------------------------*/

CHAR    ucMBSlaveID[256];
USHORT   usMBSlaveIDLen;

CHAR ucMBFileTransfer[256];
USHORT   usMBFileTransferLen;

/* ----------------------- Start implementation -----------------------------*/
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
eMBErrorCode
eMBSetSlaveID( UCHAR ucSlaveID, BOOL xIsRunning,
               UCHAR const *pucAdditional, USHORT usAdditionalLen )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* the first byte and second byte in the buffer is reserved for
     * the parameter ucSlaveID and the running flag. The rest of
     * the buffer is available for additional data. */
    if( usAdditionalLen + 2 < MB_FUNC_OTHER_REP_SLAVEID_BUF )
    {
        usMBSlaveIDLen = 0;
        ucMBSlaveID[usMBSlaveIDLen++] = ucSlaveID;
        ucMBSlaveID[usMBSlaveIDLen++] = ( UCHAR )( xIsRunning ? 0xFF : 0x00 );
        if( usAdditionalLen > 0 )
        {
            memcpy( &ucMBSlaveID[usMBSlaveIDLen], pucAdditional,
                    ( size_t )usAdditionalLen );
            usMBSlaveIDLen += usAdditionalLen;
        }
    }
    else
    {
        eStatus = MB_ENORES;
    }
    return eStatus;
}
#endif

eMBException
eMBFuncReportSlaveID( UCHAR * pucFrame, USHORT * usLen )
{
	// TODO Chiebao
    usMBSlaveIDLen = *usLen;

    for(int i = 0; i < (usMBSlaveIDLen + 2); i++)  //  + 2 bytes CRC
    	ucMBSlaveID[i] =  *(pucFrame+i);

    return MB_EX_NONE;
}


eMBMasterReqErrCode
eMBMAsterReqReportSlaveId(UCHAR ucSndAddr,  LONG lTimeOut)
{
    UCHAR                 *ucMBFrame;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
	else if ( xMBMasterRunResTake( lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
	else
	{
		vMBMasterGetPDUSndBuf(&ucMBFrame);
		vMBMasterSetDestAddress(ucSndAddr);
		ucMBFrame[MB_PDU_FUNC_OFF] = MB_FUNC_OTHER_REPORT_SLAVEID;
		vMBMasterSetPDUSndLength(MB_PDU_SIZE_MIN );
		( void ) xMBMasterPortEventPost( EV_MASTER_FRAME_SENT);
		eErrStatus = eMBMasterWaitRequestFinish( );
	}
    return eErrStatus;
}


eMBException
eMBFuncFileTransfer( UCHAR * pucFrame, USHORT * usLen )
{
    usMBFileTransferLen = *usLen;

    for(int i = 0; i < (usMBFileTransferLen + 2); i++) {  //  + 2 bytes CRC
        ucMBFileTransfer[i] =  *(pucFrame+i);
    }
    return MB_EX_NONE;
}

eMBMasterReqErrCode
eMBMAsterReqFileTransfer(UCHAR ucSndAddr, UCHAR* databuf, USHORT datalen,  LONG lTimeOut)
{
    UCHAR                 *ucMBFrame;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
	else if ( xMBMasterRunResTake( lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
	else
	{
		vMBMasterGetPDUSndBuf(&ucMBFrame);
		vMBMasterSetDestAddress(ucSndAddr);
		for (USHORT i=0; i<datalen-1; i++)
			ucMBFrame[i] = databuf[i+1];

		vMBMasterSetPDUSndLength(datalen - 1);
		xMBMasterPortEventPost( EV_MASTER_FRAME_SENT);
		eErrStatus = eMBMasterWaitRequestFinish( );
		printf("eMBMasterWaitRequestFinish eErrStatus %d\n", eErrStatus);
	}
    return eErrStatus;
}
