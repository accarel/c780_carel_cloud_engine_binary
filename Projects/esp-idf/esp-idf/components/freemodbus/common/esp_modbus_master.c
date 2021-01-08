/* Copyright 2018 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "esp_err.h"            // for esp_err_t
#include "mbc_master.h"         // for master interface define
#include "esp_modbus_master.h"  // for public interface defines
#include "mbc_serial_master.h"      // for create function of the port
#include "esp_modbus_callbacks.h"   // for callback functions

#include "mbutils.h"				// CHIEBAO A.


extern USHORT param_buffer[2];   	// CHIEBAO A.
extern eMBErrorCode retError;


//#define __STRICT_MASTER_MODBUS    // CHIEBAO A.

// This file implements public API for Modbus master controller. 
// These functions are wrappers for interface functions of the controller
static mb_master_interface_t* master_interface_ptr = NULL;

/**
 * Initialization of Modbus controller resources
 */
esp_err_t mbc_master_init(mb_port_type_t port_type, void** handler)
{
    void* port_handler = NULL;
    esp_err_t error = ESP_ERR_NOT_SUPPORTED;
    switch(port_type)
    {
    case MB_PORT_SERIAL_MASTER:
        error = mbc_serial_master_create(port_type, &port_handler);
        break;
    case MB_PORT_TCP_MASTER:
        // TCP MAster is not yet supported
        //error = mbc_tcp_master_create(port_type, &port_handler);
        return ESP_ERR_NOT_SUPPORTED;
    default:
        return ESP_ERR_NOT_SUPPORTED;
    }
    MB_MASTER_CHECK((port_handler != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface initialization failure, error=(0x%x), port type=(0x%x).",
                    (uint16_t)error, (uint16_t)port_type);

    if ((port_handler != NULL) && (error == ESP_OK)) {
        master_interface_ptr = (mb_master_interface_t*) port_handler;
        *handler = port_handler;
    }
    
    return  error;
}

/**
 * Modbus controller destroy function
 */
esp_err_t mbc_master_destroy()
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->destroy != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->destroy();
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master destroy failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

esp_err_t mbc_master_get_cid_info(uint16_t cid, const mb_parameter_descriptor_t** param_info)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->get_cid_info != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->get_cid_info(cid, param_info);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master get cid info failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Get parameter data for corresponding characteristic
 */
esp_err_t mbc_master_get_parameter(uint16_t cid, char* name, uint8_t* value, uint8_t* type)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->get_parameter != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->get_parameter(cid, name, value, type);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master get parameter failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Send custom Modbus request defined as mb_param_request_t structure
 */
esp_err_t mbc_master_send_request(mb_param_request_t* request, void* data_ptr)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->send_request != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->send_request(request, data_ptr);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master get parameter failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Set Modbus parameter description table
 */
esp_err_t mbc_master_set_descriptor(const mb_parameter_descriptor_t* descriptor, const uint16_t num_elements)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->set_descriptor != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->set_descriptor(descriptor, num_elements);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master set descriptor failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Set parameter value for characteristic selected by name and cid
 */
esp_err_t mbc_master_set_parameter(uint16_t cid, char* name, uint8_t* value, uint8_t* type)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->set_parameter != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->set_parameter(cid, name, value, type);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master set parameter failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Setup Modbus controller parameters
 */
esp_err_t mbc_master_setup(void* comm_info)
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->setup != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->setup(comm_info);
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master setup failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}

/**
 * Modbus controller stack start function
 */
esp_err_t mbc_master_start()
{
    esp_err_t error = ESP_OK;
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->start != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->start();
    MB_MASTER_CHECK((error == ESP_OK), 
                    ESP_ERR_INVALID_STATE, 
                    "SERIAL master start failure error=(0x%x).", 
                    (uint16_t)error);
    return error;
}


eMBErrorCode eMBMasterRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
                            USHORT usNDiscrete)
{
    eMBErrorCode error = MB_ENOERR;

#ifdef __STRICT_MASTER_MODBUS
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_discrete != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->master_reg_cb_discrete(pucRegBuffer, usAddress, usNDiscrete);

#else

    USHORT iRegIndex, iRegBitIndex, iNReg;
    UCHAR *pucDiscreteInputBuf;
    USHORT DISCRETE_INPUT_START;
    USHORT DISCRETE_INPUT_NDISCRETES;
    USHORT usDiscreteInputStart;
    iNReg = usNDiscrete / 8 + 1;

    pucDiscreteInputBuf = (USHORT*)param_buffer;   //TO DO .. ;

    DISCRETE_INPUT_START = 0;
    DISCRETE_INPUT_NDISCRETES = 0xFFFF;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= DISCRETE_INPUT_START) && (usAddress + usNDiscrete <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
    {
        iRegIndex = (USHORT)(usAddress) / 8;
        iRegBitIndex = (USHORT)(usAddress) % 8;

        /* write current discrete values with new values from the protocol stack. */
        while (iNReg > 1)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex, 8,
                           *pucRegBuffer++);
            iNReg--;
        }
        /* last discrete */
        usNDiscrete = usNDiscrete % 8;
        /* xMBUtilSetBits has bug when ucNBits is zero */
        if (usNDiscrete != 0)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex,
                           usNDiscrete, *pucRegBuffer++);
        }
    }
    else
    {
    	error = MB_ENOREG;
    }
#endif

    retError = error;
    return error;
}

eMBErrorCode eMBMasterRegCoilsCB(UCHAR* pucRegBuffer, USHORT usAddress,
        USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;
#ifdef __STRICT_MASTER_MODBUS
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_coils != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->master_reg_cb_coils(pucRegBuffer, usAddress,
                                                        usNCoils, eMode);
#else

	USHORT iRegIndex, iRegBitIndex, iNReg;
	UCHAR *pucCoilBuf;
	USHORT COIL_START;
	USHORT COIL_NCOILS;


	iNReg = usNCoils / 8 + 1;

	pucCoilBuf = param_buffer;

	COIL_START = 0;
	COIL_NCOILS = 0xFFFF;


	/* if mode is read,the master will write the received date to buffer. */
	eMode = MB_REG_WRITE;

	/* it already plus one in modbus function method. */
	usAddress--;

    if ((usAddress >= COIL_START) && (usAddress + usNCoils <= COIL_START + COIL_NCOILS))
    {
        iRegIndex = (USHORT)(usAddress) / 8;

        iRegBitIndex = (USHORT)(usAddress) % 8;

        switch (eMode)
        {
            /* read current coil values from the protocol stack. */
        case MB_REG_READ:
            while (iNReg > 0)
            {
                *pucRegBuffer++ = xMBUtilGetBits(&pucCoilBuf[iRegIndex++],
                                                 iRegBitIndex, 8);
                iNReg--;
            }
            pucRegBuffer--;
            /* last coils */
            usNCoils = usNCoils % 8;
            /* filling zero to high bit */
            *pucRegBuffer = *pucRegBuffer << (8 - usNCoils);
            *pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
            break;

        /* write current coil values with new values from the protocol stack. */
        case MB_REG_WRITE:
            while (iNReg > 1)
            {
                xMBUtilSetBits(&pucCoilBuf[iRegIndex++], iRegBitIndex, 8,
                               *pucRegBuffer++);
                iNReg--;
            }
            /* last coils */
            usNCoils = usNCoils % 8;
            /* xMBUtilSetBits has bug when ucNBits is zero */
            if (usNCoils != 0)
            {
                xMBUtilSetBits(&pucCoilBuf[iRegIndex++], iRegBitIndex, usNCoils, *pucRegBuffer++);
            }
            break;
        }
    }
    else
    {
    	error = MB_ENOREG;
    }
#endif

	retError = error;
    return error;
}



eMBErrorCode eMBMasterRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode error = MB_ENOERR;

#ifdef __STRICT_MASTER_MODBUS
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_holding != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->master_reg_cb_holding(pucRegBuffer, usAddress,
                                                        usNRegs, eMode);
#else
    	    USHORT iRegIndex;
    	    USHORT *pusRegHoldingBuf;
    	    USHORT REG_HOLDING_START;
    	    USHORT REG_HOLDING_NREGS;
    	    USHORT usRegHoldStart;

    	    pusRegHoldingBuf = param_buffer;

    	    REG_HOLDING_START = 0;
    	    REG_HOLDING_NREGS = 0xFFFF;

    	    /* if mode is read, the master will write the received date to buffer. */
    	    eMode = MB_REG_WRITE;

    	    /* it already plus one in modbus function method. */
    	    usAddress--;

    	    if ((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    	    {
    	        iRegIndex = 0;		//usAddress-1;
    	        switch (eMode)
    	        {
    	        /* read current register values from the protocol stack. */
    	        case MB_REG_READ:
    	            while (usNRegs > 0)
    	            {
    	                *pucRegBuffer++ = (UCHAR)(pusRegHoldingBuf[0] >> 8);
    	                *pucRegBuffer++ = (UCHAR)(pusRegHoldingBuf[0] & 0xFF);

    	                iRegIndex++;
    	                usNRegs--;
    	            }
    	            break;
    	        /* write current register values with new values from the protocol stack. */
    	        case MB_REG_WRITE:
    	            while (usNRegs > 0)
    	            {
    	                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;          //usNRegs-1
    	                pusRegHoldingBuf[iRegIndex]|= *pucRegBuffer++;               // usNRegs-1

    	                iRegIndex++;
    	                usNRegs--;
    	            }
    	            break;
    	        }
    	    }
    	    else
    	    {
    	    	error = MB_ENOREG;
    	    }
#endif

    retError = error;
    return error;
}

eMBErrorCode eMBMasterRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
                                USHORT usNRegs)
{
    eMBErrorCode error = MB_ENOERR;
#ifdef __STRICT_MASTER_MODBUS
    MB_MASTER_CHECK((master_interface_ptr != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    MB_MASTER_CHECK((master_interface_ptr->master_reg_cb_input != NULL),
                    ESP_ERR_INVALID_STATE,
                    "Master interface is not correctly initialized.");
    error = master_interface_ptr->master_reg_cb_input(pucRegBuffer, usAddress, usNRegs);
#else

	USHORT iRegIndex;
	USHORT *pusRegInputBuf;
	USHORT REG_INPUT_START;
	USHORT REG_INPUT_NREGS;


	pusRegInputBuf = (USHORT*)param_buffer;

	REG_INPUT_START = 0;
	REG_INPUT_NREGS = 0xFFFF;


	/* it already plus one in modbus function method. */
	usAddress--;

	if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
	{
		iRegIndex = 0;  //usAddress;
		while (usNRegs > 0)
		{
			pusRegInputBuf[iRegIndex] = *pucRegBuffer++ << 8;    // iRegIndex
			pusRegInputBuf[iRegIndex] |= *pucRegBuffer++;        // iRegIndex

			iRegIndex++;
			usNRegs--;
		}
	}
	else
	{
		error = MB_ENOREG;
	}
#endif

	retError = error;
    return error;
}






