/**
 ******************************************************************************
 * @file    XNucleo6180XA1.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    13-April-2015
 * @brief   Implementation file for the X_NUCLEO_VL6180XA1 singleton class
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "XNucleo6180XA1.h"


/* Static variables ----------------------------------------------------------*/

XNucleo6180XA1 *XNucleo6180XA1::_instance = NULL;


/* Methods -------------------------------------------------------------------*/

XNucleo6180XA1 *XNucleo6180XA1::instance(DevI2C *ext_i2c)
{
    if (_instance==NULL) {
        _instance=new XNucleo6180XA1(ext_i2c);
    } else {
        VL6180X_ErrLog("Failed to create X_NUCLEO_6180XA1 instance\n\r");
    }
    return _instance;
}

XNucleo6180XA1 *XNucleo6180XA1::instance(DevI2C *ext_i2c, 
                                         PinName gpio1_top, PinName gpio1_bottom,
                                         PinName gpio1_left, PinName gpio1_right)
{
    if (_instance==NULL) {
        //_instance=new XNucleo6180XA1(ext_i2c);
        _instance=new XNucleo6180XA1(ext_i2c, gpio1_top, gpio1_bottom, gpio1_left, gpio1_right);
    } else {
        VL6180X_ErrLog("Failed to create X_NUCLEO_6180XA1 instance\n\r");
    }
    return _instance;
}

int XNucleo6180XA1::init_board()
{	
    int status, n_dev=0; uint8_t sensor_address;

    if (sensor_top) {
        sensor_top->off();
    }
    if (sensor_bottom) {
        sensor_bottom->off();
    }
    if (sensor_left) {
        sensor_left->off();
    }
    if (sensor_right) {
        sensor_right->off();
    }

    sensor_address = NEW_SENSOR_TOP_ADDRESS;
    status=sensor_top->init(&sensor_address);
    if (status) {
        printf("Error: Mandatory top sensor fail, Init failed!\n\r");
        if (sensor_top !=NULL) {
            delete sensor_top;
            sensor_top=NULL;
        }
        if (sensor_left !=NULL) {
            delete sensor_left;
            sensor_left=NULL;
        }
        if (sensor_bottom !=NULL) {
            delete sensor_bottom;
            sensor_bottom=NULL;
        }
        if (sensor_right !=NULL) {
            delete sensor_right;
            sensor_right=NULL;
        }
        n_dev=0;
        return 1;
    } else {
        printf("Sensor top present\n\r");
        n_dev++;
    }

    sensor_address = NEW_SENSOR_BOTTOM_ADDRESS;
    status=1;
    if (sensor_bottom) {
        status=sensor_bottom->init(&sensor_address);
    }
    if (status)
    {
        printf("Sensor bottom not present\n\r");
        if (sensor_bottom !=NULL) {
            delete sensor_bottom;
            sensor_bottom=NULL;
        }
    } else {
        printf("Sensor bottom present\n\r");
        n_dev++;
    }

    sensor_address = NEW_SENSOR_LEFT_ADDRESS;
    status=1;
    if (sensor_left) {
        status=sensor_left->init(&sensor_address);
    }
    if (status) {
        printf("Sensor left not present\n\r");
        if (sensor_left !=NULL) {
            delete sensor_left;
            sensor_left=NULL;
        }
    } else {
        printf("Sensor left present\n\r");
        n_dev++;
    }

    sensor_address = NEW_SENSOR_RIGHT_ADDRESS;
    status=1;
    if (sensor_right) {
        status=sensor_right->init(&sensor_address);
    }
    if (status) {
        printf("Sensor right not present\n\r");
        if (sensor_right!=NULL) {
            delete sensor_right;
            sensor_right=NULL;
        }
    } else {
        printf("Sensor right present\n\r");
        n_dev++;
    }

    if (n_dev==0) {
        return 1;
    } else {
        return 0;
    }
}
