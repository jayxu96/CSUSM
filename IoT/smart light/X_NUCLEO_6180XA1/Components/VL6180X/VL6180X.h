/**
 ******************************************************************************
 * @file    VL6180X.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    9-November-2015
 * @brief   Header file for component VL6180X
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
 *       without specific prior written permission.
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


#ifndef __VL6180X_CLASS_H
#define __VL6180X_CLASS_H


/* Includes ------------------------------------------------------------------*/

#include "RangeSensor.h"
#include "LightSensor.h"
#include "DevI2C.h" 
//#include "vl6180x_api.h"
#include "vl6180x_cfg.h"
#include "vl6180x_def.h"
#include "vl6180x_types.h"
#include "vl6180x_platform.h"
#include "STMPE1600.h"


/* Definitions ---------------------------------------------------------------*/

/**
 * @brief Clear error interrupt
 *
 * @param dev    The device
 * @return  0    On success
 */
#define VL6180X_ClearErrorInterrupt(dev) VL6180X_ClearInterrupt(dev, INTERRUPT_CLEAR_ERROR)

/**
 * @brief Clear All interrupt causes (als+range+error)
 *
 * @param dev    The device
 * @return  0    On success
 */
#define VL6180X_ClearAllInterrupt(dev) VL6180X_ClearInterrupt(dev, INTERRUPT_CLEAR_ERROR|INTERRUPT_CLEAR_RANGING|INTERRUPT_CLEAR_ALS)

/**
 * Default device address
 */
#define DEFAULT_DEVICE_ADDRESS		0x29

/* Types ---------------------------------------------------------------------*/

/* data struct containing range measure, light measure and type of error provided to the user
   in case of invalid data range_mm=0xFFFFFFFF and lux=0xFFFFFFFF */	
typedef struct MeasureData 
{
   uint32_t range_mm;
   uint32_t lux;
   uint32_t range_error;
   uint32_t als_error;
   uint32_t int_error;
} measure_data_t;

/* sensor operating modes */ 
typedef enum
{
   range_single_shot_polling=1,
   als_single_shot_polling,
   range_continuous_polling,
   als_continuous_polling,
   range_continuous_interrupt,
   als_continuous_interrupt,
   interleaved_mode_interrupt,
   range_continuous_polling_low_threshold,
   range_continuous_polling_high_threshold,
   range_continuous_polling_out_of_window,
   als_continuous_polling_low_threshold,
   als_continuous_polling_high_threshold,
   als_continuous_polling_out_of_window,
   range_continuous_interrupt_low_threshold,
   range_continuous_interrupt_high_threshold,
   range_continuous_interrupt_out_of_window,
   als_continuous_interrupt_low_threshold,
   als_continuous_interrupt_high_threshold,
   als_continuous_interrupt_out_of_window,
   range_continuous_als_single_shot,
   range_single_shot_als_continuous,
} operating_mode_t;


/* Classes -------------------------------------------------------------------*/

/**
 * Class representing a VL6180X sensor component
 */
class VL6180X : public RangeSensor, public LightSensor
{
public:
    /** Constructor 1 (DigitalOut)
     * @param[in] &i2c device I2C to be used for communication
     * @param[in] &pin Mbed DigitalOut pin to be used as component GPIO_0 CE
     * @param[in] &pin_gpio1 pin Mbed InterruptIn PinName to be used as component GPIO_1 INT
     * @param[in] DevAddr device address, 0x29 by default  
     */
    VL6180X(DevI2C &i2c, DigitalOut &pin, PinName pin_gpio1, uint8_t DevAddr=DEFAULT_DEVICE_ADDRESS) : RangeSensor(), LightSensor(), _dev_i2c(&i2c), _gpio0(&pin)
    {
        _my_device.I2cAddr=DevAddr;		 
        _my_device.Present=0;
        _my_device.Ready=0;
        _device=&_my_device;;
        _expgpio0=NULL;
        if (pin_gpio1 != NC) {
        	_gpio1Int = new InterruptIn(pin_gpio1);
		} else {
			_gpio1Int = NULL;
		}
    }  
    /** Constructor 2 (STMPE1600DigiOut)
     * @param[in] i2c device I2C to be used for communication
     * @param[in] &pin Gpio Expander STMPE1600DigiOut pin to be used as component GPIO_0 CE
     * @param[in] pin_gpio1 pin Mbed InterruptIn PinName to be used as component GPIO_1 INT
     * @param[in] device address, 0x29 by default  
     */		
    VL6180X(DevI2C &i2c, STMPE1600DigiOut &pin, PinName pin_gpio1, uint8_t DevAddr=DEFAULT_DEVICE_ADDRESS) : RangeSensor(), LightSensor(), _dev_i2c(&i2c), _expgpio0(&pin)
    {
        _my_device.I2cAddr=DevAddr;		 
        _my_device.Present=0;
        _my_device.Ready=0;
        _device=&_my_device;
        _gpio0=NULL;		
        if (pin_gpio1 != NC) {
       		_gpio1Int = new InterruptIn(pin_gpio1);
   		} else {
   			_gpio1Int = NULL;
		}
    }  	 
    
    /** Destructor
     */
    virtual ~VL6180X() { 
    	if (_gpio1Int != NULL) {
    		delete _gpio1Int;
		}
    }  
    
    /* warning: VL6180X class inherits from GenericSensor, RangeSensor and LightSensor, that haven`t a destructor.
       The warning should request to introduce a virtual destructor to make sure to delete the object */

	/*** Interface Methods ***/	
	/*** High level API ***/		
	/**
	 * @brief       PowerOn the sensor
	 * @return      void
	 */		
    /* turns on the sensor */		 
    void on(void)
    {
        if (_gpio0) {
	    	*_gpio0=1;
        } else if(_expgpio0) {
	    	*_expgpio0=1;
	    }
        _my_device.I2cAddr=DEFAULT_DEVICE_ADDRESS;
        _my_device.Ready=0;       
    } 

	/**
	 * @brief       PowerOff the sensor
	 * @return      void
	 */		
    /* turns off the sensor */
    void off(void) 
    {
        if (_gpio0) {
	    	*_gpio0=0;
        } else if(_expgpio0) {
	    	*_expgpio0=0;	
        }
        _my_device.I2cAddr=DEFAULT_DEVICE_ADDRESS;
        _my_device.Ready=0;       
    }   
    
	/**
	 * @brief       Start the measure indicated by operating mode
	 * @param[in]   operating_mode specifies requested measure 
	 * @param[in]   fptr specifies call back function must be !NULL in case of interrupt measure	 
	 * @param[in]   low specifies measure low threashold in Lux or in mm according to measure
	 * @param[in]   high specifies measure high threashold in Lux or in mm according to measure
	 * @return      0 on Success
	 */					     
    int start_measurement(operating_mode_t operating_mode, void (*fptr)(void), uint16_t low, uint16_t high);

	/**
	 * @brief       Get results for the measure indicated by operating mode
	 * @param[in]   operating_mode specifies requested measure results
	 * @param[out]  Data pointer to the measure_data_t structure to read data in to
	 * @return      0 on Success
	 */					         
    int get_measurement(operating_mode_t operating_mode, measure_data_t *Data);		

	/**
	 * @brief       Stop the currently running measure indicate by operating_mode
	 * @param[in]   operating_mode specifies requested measure to stop
	 * @return      0 on Success
	 */					             
    int stop_measurement(operating_mode_t operating_mode);
     
 	/**
	 * @brief       Interrupt handling func to be called by user after an INT is occourred
	 * @param[in]   opeating_mode indicating the in progress measure
	 * @param[out]  Data pointer to the measure_data_t structure to read data in to
	 * @return      0 on Success
	 */					          		
    int handle_irq(operating_mode_t operating_mode, measure_data_t *Data);    

	/**
	 * @brief       Enable interrupt measure IRQ
	 * @return      0 on Success
	 */					     
    void enable_interrupt_measure_detection_irq(void) 
    {
		if (_gpio1Int != NULL) {
			_gpio1Int->enable_irq();
		}
    }

	/**
	 * @brief       Disable interrupt measure IRQ
	 * @return      0 on Success
	 */					          
    void disable_interrupt_measure_detection_irq(void) 
    {
		if (_gpio1Int != NULL) {
			_gpio1Int->disable_irq();
		}
    }
	/*** End High level API ***/	          
	
	/**
	 * @brief       Attach a function to call when an interrupt is detected, i.e. measurement is ready
	 * @param[in]   fptr pointer to call back function to be called whenever an interrupt occours
	 * @return      0 on Success
	 */					                  
    void attach_interrupt_measure_detection_irq(void (*fptr)(void))
    {
		if (_gpio1Int != NULL) {
			_gpio1Int->rise(fptr);
		}
    }
    
	/**
	 * @brief       Check the sensor presence
	 * @return      1 when device is present
	 */						
    unsigned present()
    {
        return _device->Present;
    }
		
    /** Wrapper functions */	
	/** @defgroup api_init Init functions
	 *  @brief    API init functions
	 *  @ingroup api_hl
	 *  @{  
	 */
	/**
	 * @brief Wait for device booted after chip enable (hardware standby)
	 * @par Function Description
	 * After Chip enable Application you can also simply wait at least 1ms to ensure device is ready
	 * @warning After device chip enable (_gpio0) de-asserted  user must wait gpio1 to get asserted (hardware standby).
	 * or wait at least 400usec prior to do any low level access or api call .
	 *
	 * This function implements polling for standby but you must ensure 400usec from chip enable passed\n
	 * @warning if device get prepared @a VL6180X_Prepare() re-using these function can hold indefinitely\n
	 *
	 * @param 		void
	 * @return     0 on success
	 */
    int wait_device_booted()
    {
        return VL6180X_WaitDeviceBooted(_device);
    }

	/**
	 *
	 * @brief One time device initialization
	 *
	 * To be called once and only once after device is brought out of reset (Chip enable) and booted see @a VL6180X_WaitDeviceBooted()
	 *
	 * @par Function Description
	 * When not used after a fresh device "power up" or reset, it may return @a #CALIBRATION_WARNING
	 * meaning wrong calibration data may have been fetched from device that can result in ranging offset error\n
	 * If application cannot execute device reset or need to run VL6180X_InitData  multiple time
	 * then it  must ensure proper offset calibration saving and restore on its own
	 * by using @a VL6180X_GetOffsetCalibrationData() on first power up and then @a VL6180X_SetOffsetCalibrationData() all all subsequent init
	 *
	 * @param void
	 * @return     0 on success,  @a #CALIBRATION_WARNING if failed
	*/
	virtual int init(void * NewAddr)
	{
	    int status;
	 
	 	off();
	    on();
	   
	    status=VL6180X_WaitDeviceBooted(_device);
	    if(status) {
	        VL6180X_ErrLog("WaitDeviceBooted fail\n\r");      
	    }  
	    status=IsPresent();
	    if(!status) {
			_device->Present=1;
	        VL6180X_InitData(_device);
	        if(status) {
	            printf("Failed to init VL6180X sensor!\n\r");
	            return status;
	        }
	        status=prepare();
	        if(status) {
	            printf("Failed to prepare VL6180X!\n\r");
	            return status;
	        }
	        if(*(uint8_t*)NewAddr!=DEFAULT_DEVICE_ADDRESS) {
	            status=set_i2c_address(*(uint8_t*)NewAddr);
	            if(status) {
	                printf("Failed to change I2C address!\n\r");
	                return status;
	            }
	        }
	        _device->Ready=1;
		}
		return status; 
	}

	/**
	 * @brief Configure GPIO1 function and set polarity.
	 * @par Function Description
	 * To be used prior to arm single shot measure or start  continuous mode.
	 *
	 * The function uses @a VL6180X_SetupGPIOx() for setting gpio 1.
	 * @warning  changing polarity can generate a spurious interrupt on pins.
	 * It sets an interrupt flags condition that must be cleared to avoid polling hangs. \n
	 * It is safe to run VL6180X_ClearAllInterrupt() just after.
	 *
	 * @param IntFunction   The interrupt functionality to use one of :\n
	 *  @a #GPIOx_SELECT_OFF \n
	 *  @a #GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT
	 * @param ActiveHigh  The interrupt line polarity see ::IntrPol_e
	 *      use @a #INTR_POL_LOW (falling edge) or @a #INTR_POL_HIGH (rising edge)
	 * @return 0 on success
	 */		
    int setup_gpio_1(uint8_t InitFunction, int ActiveHigh)
    {
		return VL6180X_SetupGPIO1(_device, InitFunction, ActiveHigh);
    }

	/**
	 * @brief  Prepare device for operation
	 * @par Function Description
	 * Does static initialization and reprogram common default settings \n
	 * _device is prepared for new measure, ready single shot ranging or ALS typical polling operation\n
	 * After prepare user can : \n
	 * @li Call other API function to set other settings\n
	 * @li Configure the interrupt pins, etc... \n
	 * @li Then start ranging or ALS operations in single shot or continuous mode
	 *
	 * @param void
	 * @return      0 on success
	 */		
    int prepare()
    {
		return VL6180X_Prepare(_device);
    }

	/**
	 * @brief Start continuous ranging mode
	 *
	 * @details End user should ensure device is in idle state and not already running
	 * @return      0 on success
	 */		
    int range_start_continuous_mode()
    {
		return VL6180X_RangeStartContinuousMode(_device);
    }

	/**
	 * @brief Start single shot ranging measure
	 *
	 * @details End user should ensure device is in idle state and not already running
	 * @return      0 on success 
	 */		
    int range_start_single_shot()
    {
		return VL6180X_RangeStartSingleShot(_device);
    }

	/**
	 * @brief Set maximum convergence time
	 *
	 * @par Function Description
	 * Setting a low convergence time can impact maximal detectable distance.
	 * Refer to VL6180X Datasheet Table 7 : Typical range convergence time.
	 * A typical value for up to x3 scaling is 50 ms
	 *
	 * @param MaxConTime_msec
	 * @return 0 on success. <0 on error. >0 for calibration warning status
	 */		
    int range_set_max_convergence_time(uint8_t MaxConTime_msec)
    {
        return VL6180X_RangeSetMaxConvergenceTime(_device, MaxConTime_msec);
    }

	/**
	 * @brief Single shot Range measurement in polling mode.
	 *
	 * @par Function Description
	 * Kick off a new single shot range  then wait for ready to retrieve it by polling interrupt status \n
	 * Ranging must be prepared by a first call to  @a VL6180X_Prepare() and it is safer to clear  very first poll call \n
	 * This function reference VL6180X_PollDelay(dev) porting macro/call on each polling loop,
	 * but PollDelay(dev) may never be called if measure in ready on first poll loop \n
	 * Should not be use in continuous mode operation as it will stop it and cause stop/start misbehaviour \n
	 * \n This function clears Range Interrupt status , but not error one. For that uses  @a VL6180X_ClearErrorInterrupt() \n
	 * This range error is not related VL6180X_RangeData_t::errorStatus that refer measure status \n
	 * 
	 * @param pRangeData   Will be populated with the result ranging data @a  VL6180X_RangeData_t
	 * @return 0 on success , @a #RANGE_ERROR if device reports an error case in it status (not cleared) use
	 *
	 * \sa ::VL6180X_RangeData_t
	 */		
    int range_poll_measurement(VL6180X_RangeData_t *pRangeData)
    {
        return VL6180X_RangePollMeasurement(_device, pRangeData);
    }

	/**
	 * @brief Check for measure readiness and get it if ready
	 *
	 * @par Function Description
	 * Using this function is an alternative to @a VL6180X_RangePollMeasurement() to avoid polling operation. This is suitable for applications
	 * where host CPU is triggered on a interrupt (not from VL6180X) to perform ranging operation. In this scenario, we assume that the very first ranging
	 * operation is triggered by a call to @a VL6180X_RangeStartSingleShot(). Then, host CPU regularly calls @a VL6180X_RangeGetMeasurementIfReady() to
	 * get a distance measure if ready. In case the distance is not ready, host may get it at the next call.\n
	 *
	 * @warning 
	 * This function does not re-start a new measurement : this is up to the host CPU to do it.\n 
	 * This function clears Range Interrupt for measure ready , but not error interrupts. For that, uses  @a VL6180X_ClearErrorInterrupt() \n
	 *
	 * @param pRangeData  Will be populated with the result ranging data if available
	 * @return  0 when measure is ready pRange data is updated (untouched when not ready),  >0 for warning and @a #NOT_READY if measurement not yet ready, <0 for error @a #RANGE_ERROR if device report an error,
	 */		
    int _range_get_measurement_if_ready(VL6180X_RangeData_t *pRangeData)
    {
        return VL6180X_RangeGetMeasurementIfReady(_device, pRangeData);
    }

	/**
	 * @brief Retrieve range measurements set  from device
	 *
	 * @par Function Description
	 * The measurement is made of range_mm status and error code @a VL6180X_RangeData_t \n
	 * Based on configuration selected extra measures are included.
	 *
	 * @warning should not be used in continuous if wrap around filter is active \n
	 * Does not perform any wait nor check for result availability or validity.
	 *\sa VL6180X_RangeGetResult for "range only" measurement
	 *
	 * @param pRangeData  Pointer to the data structure to fill up
	 * @return            0 on success
	 */		
    int range_get_measurement(VL6180X_RangeData_t *pRangeData)
    {
        return VL6180X_RangeGetMeasurement(_device, pRangeData);
    }

	/**
	 * @brief Get a single distance measure result
	 *
	 * @par Function Description
	 * It can be called after having initialized a component. It start a single 
	 * distance measure in polling mode and wait until the measure is finisched. 
	 * The function block until the measure is finished, it can blocks indefinitely 
	 * in case the measure never ends for any reason \n
	 *
	 * @param pi_data  Pointer to distance
	 * @return 0 on success
	 */		
    virtual int get_distance(uint32_t *pi_data)
    {
        int status=0; 
        LOG_FUNCTION_START("");
        status=start_measurement(range_single_shot_polling, NULL, NULL, NULL);
        if (!status) {
		    range_wait_device_ready(2000);				 
            for (status=1; status!=0; status=VL6180X_RangeGetResult(_device, pi_data));
        }
        stop_measurement(range_single_shot_polling);
        range_wait_device_ready(2000);
        LOG_FUNCTION_END(status);

        return status;
    }
			
	/**
	 * @brief Configure ranging interrupt reported to application
	 *
	 * @param ConfigGpioInt  Select ranging report\n select one (and only one) of:\n
	 *   @a #CONFIG_GPIO_INTERRUPT_DISABLED \n
	 *   @a #CONFIG_GPIO_INTERRUPT_LEVEL_LOW \n
	 *   @a #CONFIG_GPIO_INTERRUPT_LEVEL_HIGH \n
	 *   @a #CONFIG_GPIO_INTERRUPT_OUT_OF_WINDOW \n
	 *   @a #CONFIG_GPIO_INTERRUPT_NEW_SAMPLE_READY
	 * @return   0 on success
	 */
    int range_config_interrupt(uint8_t ConfigGpioInt)
    {
        return VL6180X_RangeConfigInterrupt(_device, ConfigGpioInt);
    }

	/**
	 * @brief Return ranging error interrupt status
	 *
	 * @par Function Description
	 * Appropriate Interrupt report must have been selected first by @a VL6180X_RangeConfigInterrupt() or @a  VL6180X_Prepare() \n
	 *
	 * Can be used in polling loop to wait for a given ranging event or in interrupt to read the trigger \n
	 * Events triggers are : \n
	 * @a #RES_INT_STAT_GPIO_LOW_LEVEL_THRESHOLD \n
	 * @a #RES_INT_STAT_GPIO_HIGH_LEVEL_THRESHOLD \n
	 * @a #RES_INT_STAT_GPIO_OUT_OF_WINDOW \n (RES_INT_STAT_GPIO_LOW_LEVEL_THRESHOLD|RES_INT_STAT_GPIO_HIGH_LEVEL_THRESHOLD)
	 * @a #RES_INT_STAT_GPIO_NEW_SAMPLE_READY \n
	 *
	 * @sa IntrStatus_t
	 * @param pIntStatus Pointer to status variable to update
	 * @return           0 on success
	 */		
    int range_get_interrupt_status(uint8_t *pIntStatus)
    {
        return VL6180X_RangeGetInterruptStatus(_device, pIntStatus);
    }

	/**
	 * @brief   Run a single ALS measurement in single shot polling mode
	 *
	 * @par Function Description
	 * Kick off a new single shot ALS then wait new measurement ready to retrieve it ( polling system interrupt status register for als) \n
	 * ALS must be prepared by a first call to @a VL6180X_Prepare() \n
	 * \n Should not be used in continuous or interrupt mode it will break it and create hazard in start/stop \n
	 *
	 * @param dev          The device
	 * @param pAlsData     Als data structure to fill up @a VL6180X_AlsData_t
	 * @return             0 on success
	 */
    int als_poll_measurement(VL6180X_AlsData_t *pAlsData)
    {
        return VL6180X_AlsPollMeasurement(_device, pAlsData);
    }

	/**
	 * @brief  Get actual ALS measurement
	 *
	 * @par Function Description
	 * Can be called after success status polling or in interrupt mode to retrieve ALS measurement from device \n
	 * This function doesn't perform any data ready check !
	 *
	 * @param pAlsData   Pointer to measurement struct @a VL6180X_AlsData_t
	 * @return  0 on success
	 */		
    int als_get_measurement(VL6180X_AlsData_t *pAlsData)
    {
        return VL6180X_AlsGetMeasurement(_device, pAlsData);
    }

	/**
	 * @brief  Configure ALS interrupts provide to application
	 *
	 * @param ConfigGpioInt  Select one (and only one) of : \n
	 *   @a #CONFIG_GPIO_INTERRUPT_DISABLED \n
	 *   @a #CONFIG_GPIO_INTERRUPT_LEVEL_LOW \n
	 *   @a #CONFIG_GPIO_INTERRUPT_LEVEL_HIGH \n
	 *   @a #CONFIG_GPIO_INTERRUPT_OUT_OF_WINDOW \n
	 *   @a #CONFIG_GPIO_INTERRUPT_NEW_SAMPLE_READY
	 * @return               0 on success may return #INVALID_PARAMS for invalid mode
	 */	
    int als_config_interrupt(uint8_t ConfigGpioInt)
    {
        return VL6180X_AlsConfigInterrupt(_device, ConfigGpioInt);
    }

	/**
	 * @brief Set ALS integration period
	 *
	 * @param period_ms  Integration period in msec. Value in between 50 to 100 msec is recommended\n
	 * @return           0 on success
	 */		
    int als_set_integration_period(uint16_t period_ms)
    {
        return VL6180X_AlsSetIntegrationPeriod(_device, period_ms);	
    }

	/**
	 * @brief Set ALS "inter-measurement period"
	 *
	 * @par Function Description
	 * The so call data-sheet "inter measurement period" is actually an extra inter-measurement delay
	 *
	 * @param intermeasurement_period_ms Inter measurement time in milli second\n
	 *        @warning applied value is clipped to 2550 ms\n
	 * @return           0 on success if value is
	 */		
    int als_set_inter_measurement_period(uint16_t intermeasurement_period_ms)
    {
        return VL6180X_AlsSetInterMeasurementPeriod(_device, intermeasurement_period_ms);
    }

	/**
	 * @brief Set ALS analog gain code
	 *
	 * @par Function Description
	 * ALS gain code value programmed in @a SYSALS_ANALOGUE_GAIN .
	 * @param gain  Gain code see datasheet or AlsGainLookUp for real value. Value is clipped to 7.
	 * @return  0 on success
	 */
    int als_set_analogue_gain(uint8_t gain)
    {
        return VL6180X_AlsSetAnalogueGain(_device, gain);
    }

	/**
	 * @brief Set thresholds for ALS continuous mode 
	 * @warning Threshold are raw device value not lux!
	 *
	 * @par Function Description
	 * Basically value programmed in @a SYSALS_THRESH_LOW and @a SYSALS_THRESH_HIGH registers
	 * @param low   ALS low raw threshold for @a SYSALS_THRESH_LOW
	 * @param high  ALS high raw threshold for  @a SYSALS_THRESH_HIGH
	 * @return  0 on success
	 */		
    int als_set_thresholds(uint16_t lux_threshold_low, uint16_t lux_threshold_high);

	/**
	 * Read ALS interrupt status
	 * @param pIntStatus  Pointer to status
	 * @return            0 on success
	 */
    int als_get_interrupt_status(uint8_t *pIntStatus)
    {
        return VL6180X_AlsGetInterruptStatus(_device, pIntStatus);
    }

	/**
	 * @brief Low level ranging and ALS register static settings (you should call @a VL6180X_Prepare() function instead)
	 *
	 * @return 0 on success
	 */
    int static_init()
    {
        return VL6180X_StaticInit(_device);
    }

	/**
	 * @brief Wait for device to be ready (before a new ranging command can be issued by application)
	 * @param MaxLoop    Max Number of i2c polling loop see @a #msec_2_i2cloop
	 * @return           0 on success. <0 when fail \n
	 *                   @ref VL6180X_ErrCode_t::TIME_OUT for time out \n
	 *                   @ref VL6180X_ErrCode_t::INVALID_PARAMS if MaxLop<1
	 */		
    int range_wait_device_ready(int MaxLoop)
    {
        return VL6180X_RangeWaitDeviceReady(_device, MaxLoop);
    }

	/**
	 * @brief Program Inter measurement period (used only in continuous mode)
	 *
	 * @par Function Description
	 * When trying to set too long time, it returns #INVALID_PARAMS
	 *
	 * @param InterMeasTime_msec Requires inter-measurement time in msec
	 * @return 0 on success
	 */		
    int range_set_inter_meas_period(uint32_t InterMeasTime_msec)
    {
        return VL6180X_RangeSetInterMeasPeriod(_device, InterMeasTime_msec);
    }

	/**
	 * @brief Set device ranging scaling factor
	 *
	 * @par Function Description
	 * The ranging scaling factor is applied on the raw distance measured by the device to increase operating ranging at the price of the precision.
	 * Changing the scaling factor when device is not in f/w standby state (free running) is not safe.
	 * It can be source of spurious interrupt, wrongly scaled range etc ...
	 * @warning __This  function doesns't update high/low threshold and other programmed settings linked to scaling factor__.
	 *  To ensure proper operation, threshold and scaling changes should be done following this procedure: \n
	 *  @li Set Group hold  : @a VL6180X_SetGroupParamHold() \n
	 *  @li Get Threshold @a VL6180X_RangeGetThresholds() \n
	 *  @li Change scaling : @a VL6180X_UpscaleSetScaling() \n
	 *  @li Set Threshold : @a VL6180X_RangeSetThresholds() \n
	 *  @li Unset Group Hold : @a VL6180X_SetGroupParamHold()
	 *
	 * @param scaling  Scaling factor to apply (1,2 or 3)
	 * @return         0 on success when up-scale support is not configured it fail for any
	 *                 scaling than the one statically configured.
	 */
    int upscale_set_scaling(uint8_t scaling)
    {
        return VL6180X_UpscaleSetScaling(_device, scaling);
    }

	/**
	 * @brief Get current ranging scaling factor
	 *
	 * @return    The current scaling factor
	 */				
    int upscale_get_scaling()
    {
        return VL6180X_UpscaleGetScaling(_device);
    }

	/**
	 * @brief Get the maximal distance for actual scaling
	 * @par Function Description
	 * Do not use prior to @a VL6180X_Prepare() or at least @a VL6180X_InitData()
	 *
	 * Any range value more than the value returned by this function is to be considered as "no target detected"
	 * or "no target in detectable range" \n
	 * @warning The maximal distance depends on the scaling
	 *
	 * @return    The maximal range limit for actual mode and scaling
	 */		
    uint16_t get_upper_limit()
    {
        return VL6180X_GetUpperLimit(_device);
    }

	/**
	 * @brief Apply low and high ranging thresholds that are considered only in continuous mode
	 *
	 * @par Function Description
	 * This function programs low and high ranging thresholds that are considered in continuous mode : 
	 * interrupt will be raised only when an object is detected at a distance inside this [low:high] range.  
	 * The function takes care of applying current scaling factor if any.\n
	 * To be safe, in continuous operation, thresholds must be changed under "group parameter hold" cover.
	 * Group hold can be activated/deactivated directly in the function or externally (then set 0)
	 * using /a VL6180X_SetGroupParamHold() function.
	 *
	 * @param low      Low threshold in mm
	 * @param high     High threshold in mm
	 * @param SafeHold  Use of group parameters hold to surround threshold programming.
	 * @return  0 On success
	 */		
    int range_set_thresholds(uint16_t low, uint16_t high, int SafeHold)
    {
        return VL6180X_RangeSetThresholds(_device, low, high, SafeHold);
    }

	/**
	 * @brief  Get scaled high and low threshold from device
	 *
	 * @par Function Description
	 * Due to scaling factor, the returned value may be different from what has been programmed first (precision lost).
	 * For instance VL6180X_RangeSetThresholds(dev,11,22) with scale 3
	 * will read back 9 ((11/3)x3) and 21 ((22/3)x3).
	 *
	 * @param low  scaled low Threshold ptr  can be NULL if not needed
	 * @param high scaled High Threshold ptr can be NULL if not needed
	 * @return 0 on success, return value is undefined if both low and high are NULL
	 * @warning return value is undefined if both low and high are NULL
	 */
    int range_get_thresholds(uint16_t *low, uint16_t *high)
    {
        return VL6180X_RangeGetThresholds(_device, low, high);
    }

	/**
	 * @brief Set ranging raw thresholds (scaling not considered so not recommended to use it)
	 *
	 * @param low  raw low threshold set to raw register
	 * @param high raw high threshold set to raw  register
	 * @return 0 on success
	 */			
    int range_set_raw_thresholds(uint8_t low, uint8_t high)
    {
        return VL6180X_RangeSetRawThresholds(_device, low, high);
    }

	/**
	 * @brief Set Early Convergence Estimate ratio
	 * @par Function Description
	 * For more information on ECE check datasheet
	 * @warning May return a calibration warning in some use cases
	 *
	 * @param FactorM    ECE factor M in M/D
	 * @param FactorD    ECE factor D in M/D
	 * @return           0 on success. <0 on error. >0 on warning
	 */		
    int range_set_ece_factor(uint16_t  FactorM, uint16_t FactorD)
    {
        return VL6180X_RangeSetEceFactor(_device, FactorM, FactorD);
    }

	/**
	 * @brief Set Early Convergence Estimate state (See #SYSRANGE_RANGE_CHECK_ENABLES register)
	 * @param enable    State to be set 0=disabled, otherwise enabled
	 * @return          0 on success
	 */		
    int range_set_ece_state(int enable)
    {
        return VL6180X_RangeSetEceState(_device, enable);
    }

	/**
	 * @brief Set activation state of the wrap around filter
	 * @param state New activation state (0=off,  otherwise on)
	 * @return      0 on success
	 */			
    int flter_set_state(int state)
    {
        return VL6180X_FilterSetState(_device, state);
    }

	/**
	 * Get activation state of the wrap around filter
	 * @return     Filter enabled or not, when filter is not supported it always returns 0S
	 */			
    int filter_get_state()
    {
        return VL6180X_FilterGetState(_device);
    }

	/**
	 * @brief Set activation state of  DMax computation
	 * @param state New activation state (0=off,  otherwise on)
	 * @return      0 on success
	 */		
    int d_max_set_state(int state)
    {
        return VL6180X_DMaxSetState(_device, state);
    }

	/**
	 * Get activation state of DMax computation
	 * @return     Filter enabled or not, when filter is not supported it always returns 0S
	 */		
    int d_max_get_state()
    {
        return VL6180X_DMaxGetState(_device);
    }

	/**
	 * @brief Set ranging mode and start/stop measure (use high level functions instead : @a VL6180X_RangeStartSingleShot() or @a VL6180X_RangeStartContinuousMode())
	 *
	 * @par Function Description
	 * When used outside scope of known polling single shot stopped state, \n
	 * user must ensure the device state is "idle" before to issue a new command.
	 *
	 * @param mode  A combination of working mode (#MODE_SINGLESHOT or #MODE_CONTINUOUS) and start/stop condition (#MODE_START_STOP) \n
	 * @return      0 on success
	 */		
    int range_set_system_mode(uint8_t mode)
    {
        return VL6180X_RangeSetSystemMode(_device, mode);
    }

	/** @}  */ 
	
	/** @defgroup api_ll_range_calibration Ranging calibration functions
	 *  @brief    Ranging calibration functions
	 *  @ingroup api_ll
	 *  @{  
	 */
	/**
	 * @brief Get part to part calibration offset
	 *
	 * @par Function Description
	 * Should only be used after a successful call to @a VL6180X_InitData to backup device nvm value
	 *
	 * @return part to part calibration offset from device
	 */		
    int8_t get_offset_calibration_data()
    {
        return VL6180X_GetOffsetCalibrationData(_device);
    }

	/**
	 * Set or over-write part to part calibration offset
	 * \sa VL6180X_InitData(), VL6180X_GetOffsetCalibrationData()
	 * @param offset   Offset
	 */		
    void set_offset_calibration_data(int8_t offset)
    {
        return VL6180X_SetOffsetCalibrationData(_device, offset);
    }

	/**
	 * @brief Set Cross talk compensation rate
	 *
	 * @par Function Description
	 * It programs register @a #SYSRANGE_CROSSTALK_COMPENSATION_RATE
	 *
	 * @param Rate Compensation rate (9.7 fix point) see datasheet for details
	 * @return     0 on success
	 */		
    int set_x_talk_compensation_rate(FixPoint97_t Rate)
    {
        return VL6180X_SetXTalkCompensationRate(_device, Rate);
    }
	/** @}  */
	
	/** @defgroup api_ll_als ALS functions
	 *  @brief    ALS functions
	 *  @ingroup api_ll
	 *  @{  
	 */
	
	/**
	 * @brief Wait for device to be ready for new als operation or max pollign loop (time out)
	 * @param MaxLoop    Max Number of i2c polling loop see @a #msec_2_i2cloop
	 * @return           0 on success. <0 when @a VL6180X_ErrCode_t::TIME_OUT if timed out
	 */		
    int als_wait_device_ready(int MaxLoop)
    {
        return VL6180X_AlsWaitDeviceReady(_device, MaxLoop);
    }
		
	/**
	 * @brief Set ALS system mode and start/stop measure
	 *
	 * @warning When used outside after single shot polling, \n
	 * User must ensure  the device state is ready before issuing a new command (using @a VL6180X_AlsWaitDeviceReady()). \n
	 * Non respect of this, can cause loss of interrupt or device hanging.
	 *
	 * @param mode  A combination of working mode (#MODE_SINGLESHOT or #MODE_CONTINUOUS) and start condition (#MODE_START_STOP) \n
	 * @return      0 on success
	 */		
    int als_set_system_mode(uint8_t mode)
    {
        return VL6180X_AlsSetSystemMode(_device, mode);
    }

	/** @defgroup api_ll_misc Misc functions
	 *  @brief    Misc functions
	 *  @ingroup api_ll
	 *  @{  
	 */

	/**
	 * Set Group parameter Hold state
	 *
	 * @par Function Description
	 * Group parameter holds @a #SYSTEM_GROUPED_PARAMETER_HOLD enable safe update (non atomic across multiple measure) by host
	 * \n The critical register group is composed of: \n
	 * #SYSTEM_INTERRUPT_CONFIG_GPIO \n
	 * #SYSRANGE_THRESH_HIGH \n
	 * #SYSRANGE_THRESH_LOW \n
	 * #SYSALS_INTEGRATION_PERIOD \n
	 * #SYSALS_ANALOGUE_GAIN \n
	 * #SYSALS_THRESH_HIGH \n
	 * #SYSALS_THRESH_LOW
	 *
	 *
	 * @param Hold  Group parameter Hold state to be set (on/off)
	 * @return      0 on success
	 */
    int set_group_param_hold(int Hold)
    {
        return VL6180X_SetGroupParamHold(_device, Hold);
    }		

	/**
	 * @brief Set new device i2c address
	 *
	 * After completion the device will answer to the new address programmed.
	 *
	 * @sa AN4478: Using multiple VL6180X's in a single design
	 * @param NewAddr   The new i2c address (7bit)
	 * @return          0 on success
	 */		
    int set_i2c_address(int NewAddr)
    {
		int status;
			
		status=VL6180X_SetI2CAddress(_device, NewAddr);
		if (!status) {
			_device->I2cAddr=NewAddr;
		}
        return status;
    }

	/**
	 * @brief Fully configure gpio 0/1 pin : polarity and functionality
	 *
	 * @param pin          gpio pin 0 or 1
	 * @param IntFunction  Pin functionality : either #GPIOx_SELECT_OFF or #GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT (refer to #SYSTEM_MODE_GPIO1 register definition)
	 * @param ActiveHigh   Set active high polarity, or active low see @a ::IntrPol_e
	 * @return             0 on success
	 */		
    int setup_gpio_x(int pin, uint8_t IntFunction, int ActiveHigh)
    {
        return VL6180X_SetupGPIOx(_device, pin, IntFunction, ActiveHigh);
    }

	/**
	 * @brief Set interrupt pin polarity for the given GPIO
	 *
	 * @param pin          Pin 0 or 1
	 * @param active_high  select active high or low polarity using @ref IntrPol_e
	 * @return             0 on success
	 */		
    int set_gpio_x_polarity(int pin, int active_high)
    {
        return VL6180X_SetGPIOxPolarity(_device, pin, active_high);
    }

	/**
	 * Select interrupt functionality for the given GPIO
	 *
	 * @par Function Description
	 * Functionality refer to @a SYSTEM_MODE_GPIO0
	 *
	 * @param pin            Pin to configure 0 or 1 (_gpio0 or gpio1)\nNote that _gpio0 is chip enable at power up !
	 * @param functionality  Pin functionality : either #GPIOx_SELECT_OFF or #GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT (refer to #SYSTEM_MODE_GPIO1 register definition)
	 * @return              0 on success
	 */		 
    int set_gpio_x_functionality(int pin, uint8_t functionality)
    {
        return VL6180X_SetGPIOxFunctionality(_device, pin, functionality);
    }

	/**
	 * #brief Disable and turn to Hi-Z gpio output pin
	 *
	 * @param pin  The pin number to disable 0 or 1
	 * @return     0 on success
	 */	
    int disable_gpio_x_out(int pin)
    {
        return VL6180X_DisableGPIOxOut(_device, pin);
    }

	/** @}  */
	
	/** @defgroup api_ll_intr Interrupts management functions
	 *  @brief    Interrupts management functions
	 *  @ingroup api_ll
	 *  @{  
	 */

	/**
	 * @brief     Get all interrupts cause
	 *
	 * @param status Ptr to interrupt status. You can use @a IntrStatus_t::val
	 * @return 0 on success
	 */		
    int get_interrupt_status(uint8_t *status)
    {
        return VL6180X_GetInterruptStatus(_device, status);
    }

	/**
	 * @brief Clear given system interrupt condition
	 *
	 * @par Function Description
	 * Clear given interrupt cause by writing into register #SYSTEM_INTERRUPT_CLEAR register.
	 * @param dev       The device 
	 * @param IntClear  Which interrupt source to clear. Use any combinations of #INTERRUPT_CLEAR_RANGING , #INTERRUPT_CLEAR_ALS , #INTERRUPT_CLEAR_ERROR.
	 * @return  0       On success
	 */		
    int clear_interrupt(uint8_t IntClear)
    {
        return VL6180X_ClearInterrupt(_device, IntClear );
    }

	/** @}  */
	
	/**
	 * @brief Get a single light (in Lux) measure result
	 *
	 * @par Function Description
	 * It can be called after having initialized a component. It start a single 
	 * light measure in polling mode and wait until the measure is finisched. 
	 * The function block until the measure is finished, it can blocks indefinitely 
	 * in case the measure never ends for any reason \n
	 */				
    virtual int get_lux(uint32_t *pi_data)
    {
        int status=0; 
        LOG_FUNCTION_START("");
        status = start_measurement(als_single_shot_polling, NULL, NULL, NULL);
        if (!status) {        
            als_wait_device_ready(2000);				 
            for (status=1; status!=0; status=VL6180X_AlsGetLux(_device, pi_data));
        }
        stop_measurement(als_single_shot_polling);
	    als_wait_device_ready(2000);				 			 
        LOG_FUNCTION_END(status);       

        return status;
    }

	/**
	 * @brief Start the ALS (light) measure in continous mode
	 *
	 * @par Function Description
	 * Start the ALS (light) measure in continous mode
	 * @return  0       On success
	 */						
    int als_start_continuous_mode()
    {
        return VL6180X_AlsSetSystemMode(_device, MODE_START_STOP|MODE_CONTINUOUS);
    }

	/**
	 * @brief Start the ALS (light) measure in single shot mode
	 *
	 * @par Function Description
	 * Start the ALS (light) measure in single shot mode
	 * @return  0       On success
	 */						    
    int als_start_single_shot()
    {
        return VL6180X_AlsSetSystemMode(_device, MODE_START_STOP|MODE_SINGLESHOT);
    }
		
private:		
    /* api.h functions */
    int VL6180X_WaitDeviceBooted(VL6180XDev_t dev);
    int VL6180X_InitData(VL6180XDev_t dev );
    int VL6180X_SetupGPIO1(VL6180XDev_t dev, uint8_t IntFunction, int ActiveHigh);
    int VL6180X_Prepare(VL6180XDev_t dev);
    int VL6180X_RangeStartContinuousMode(VL6180XDev_t dev);
    int VL6180X_RangeStartSingleShot(VL6180XDev_t dev);
    int VL6180X_RangeSetMaxConvergenceTime(VL6180XDev_t dev, uint8_t  MaxConTime_msec);
    int VL6180X_RangePollMeasurement(VL6180XDev_t dev, VL6180X_RangeData_t *pRangeData);
    int VL6180X_RangeGetMeasurementIfReady(VL6180XDev_t dev, VL6180X_RangeData_t *pRangeData);
    int VL6180X_RangeGetMeasurement(VL6180XDev_t dev, VL6180X_RangeData_t *pRangeData);
    int VL6180X_RangeGetResult(VL6180XDev_t dev, uint32_t *pRange_mm);
    int VL6180X_RangeConfigInterrupt(VL6180XDev_t dev, uint8_t ConfigGpioInt);
    int VL6180X_RangeGetInterruptStatus(VL6180XDev_t dev, uint8_t *pIntStatus);
    int VL6180X_AlsPollMeasurement(VL6180XDev_t dev, VL6180X_AlsData_t *pAlsData);
    int VL6180X_AlsGetMeasurement(VL6180XDev_t dev, VL6180X_AlsData_t *pAlsData);
    int VL6180X_AlsConfigInterrupt(VL6180XDev_t dev, uint8_t ConfigGpioInt);
    int VL6180X_AlsSetIntegrationPeriod(VL6180XDev_t dev, uint16_t period_ms);
    int VL6180X_AlsSetInterMeasurementPeriod(VL6180XDev_t dev,  uint16_t intermeasurement_period_ms);
    int VL6180X_AlsSetAnalogueGain(VL6180XDev_t dev, uint8_t gain);
    int VL6180X_AlsSetThresholds(VL6180XDev_t dev, uint16_t low, uint16_t high);
    int VL6180X_AlsGetInterruptStatus(VL6180XDev_t dev, uint8_t *pIntStatus);
    int VL6180X_StaticInit(VL6180XDev_t dev);
    int VL6180X_RangeWaitDeviceReady(VL6180XDev_t dev, int MaxLoop );
    int VL6180X_RangeSetInterMeasPeriod(VL6180XDev_t dev, uint32_t  InterMeasTime_msec);
    int VL6180X_UpscaleSetScaling(VL6180XDev_t dev, uint8_t scaling);
    int VL6180X_UpscaleGetScaling(VL6180XDev_t dev);
    uint16_t VL6180X_GetUpperLimit(VL6180XDev_t dev);
    int VL6180X_RangeSetThresholds(VL6180XDev_t dev, uint16_t low, uint16_t high, int SafeHold);
    int VL6180X_RangeGetThresholds(VL6180XDev_t dev, uint16_t *low, uint16_t *high);
    int VL6180X_RangeSetRawThresholds(VL6180XDev_t dev, uint8_t low, uint8_t high);
    int VL6180X_RangeSetEceFactor(VL6180XDev_t dev, uint16_t  FactorM, uint16_t FactorD);
    int VL6180X_RangeSetEceState(VL6180XDev_t dev, int enable );
    int VL6180X_FilterSetState(VL6180XDev_t dev, int state);
    int VL6180X_FilterGetState(VL6180XDev_t dev);
    int VL6180X_DMaxSetState(VL6180XDev_t dev, int state);
    int VL6180X_DMaxGetState(VL6180XDev_t dev);
    int VL6180X_RangeSetSystemMode(VL6180XDev_t dev, uint8_t mode);
    int8_t VL6180X_GetOffsetCalibrationData(VL6180XDev_t dev);
    void VL6180X_SetOffsetCalibrationData(VL6180XDev_t dev, int8_t offset);
    int VL6180X_SetXTalkCompensationRate(VL6180XDev_t dev, FixPoint97_t Rate);
    int VL6180X_AlsWaitDeviceReady(VL6180XDev_t dev, int MaxLoop );
    int VL6180X_AlsSetSystemMode(VL6180XDev_t dev, uint8_t mode); 
    int VL6180X_SetGroupParamHold(VL6180XDev_t dev, int Hold);
    int VL6180X_SetI2CAddress(VL6180XDev_t dev, uint8_t NewAddr);
    int VL6180X_SetupGPIOx(VL6180XDev_t dev, int pin, uint8_t IntFunction, int ActiveHigh);
    int VL6180X_SetGPIOxPolarity(VL6180XDev_t dev, int pin, int active_high);
    int VL6180X_SetGPIOxFunctionality(VL6180XDev_t dev, int pin, uint8_t functionality);
    int VL6180X_DisableGPIOxOut(VL6180XDev_t dev, int pin);
    int VL6180X_GetInterruptStatus(VL6180XDev_t dev, uint8_t *status);
    int VL6180X_ClearInterrupt(VL6180XDev_t dev, uint8_t IntClear );
		
    /*  Other functions defined in api.c */
    int VL6180X_RangeStaticInit(VL6180XDev_t dev); 
    int VL6180X_UpscaleRegInit(VL6180XDev_t dev);
    int VL6180X_UpscaleStaticInit(VL6180XDev_t dev); 
    int VL6180X_AlsGetLux(VL6180XDev_t dev, lux_t *pLux);
    int _UpscaleInitPatch0(VL6180XDev_t dev); 
    int VL6180X_RangeGetDeviceReady(VL6180XDev_t dev, int * Ready);
    int VL6180X_RangeSetEarlyConvergenceEestimateThreshold(VL6180XDev_t dev);
    int32_t _GetAveTotalTime(VL6180XDev_t dev); 
    int32_t _filter_Start(VL6180XDev_t dev, uint16_t m_trueRange_mm, uint16_t m_rawRange_mm, uint32_t m_rtnSignalRate, uint32_t m_rtnAmbientRate, uint16_t errorCode);
    int _filter_GetResult(VL6180XDev_t dev, VL6180X_RangeData_t *pRangeData);
    int _GetRateResult(VL6180XDev_t dev, VL6180X_RangeData_t *pRangeData); 
    int _DMax_InitData(VL6180XDev_t dev);
		
    /* Read function of the ID device */
    virtual int read_id(uint8_t *id);
    
    /* Write and read functions from I2C */
    int VL6180X_WrByte(VL6180XDev_t dev, uint16_t index, uint8_t data);
    int VL6180X_WrWord(VL6180XDev_t dev, uint16_t index, uint16_t data);
    int VL6180X_WrDWord(VL6180XDev_t dev, uint16_t index, uint32_t data);
    int VL6180X_RdByte(VL6180XDev_t dev, uint16_t index, uint8_t *data);
    int VL6180X_RdWord(VL6180XDev_t dev, uint16_t index, uint16_t *data);
    int VL6180X_RdDWord(VL6180XDev_t dev, uint16_t index, uint32_t *data);
    int VL6180X_UpdateByte(VL6180XDev_t dev, uint16_t index, uint8_t AndData, uint8_t OrData);
    int VL6180X_I2CWrite(uint8_t DeviceAddr, uint16_t RegisterAddr, uint8_t *pBuffer, uint16_t NumByteToWrite);
    int VL6180X_I2CRead(uint8_t DeviceAddr, uint16_t RegisterAddr, uint8_t *pBuffer, uint16_t NumByteToRead);
		
    int IsPresent();
    int StopRangeMeasurement(operating_mode_t operating_mode);
    int StopAlsMeasurement(operating_mode_t operating_mode);
    int GetRangeMeas(operating_mode_t operating_mode, measure_data_t *Data);	
    int GetAlsMeas(operating_mode_t operating_mode, measure_data_t *Data);
    int GetRangeAlsMeas(measure_data_t *Data);
    int RangeSetLowThreshold(uint16_t threshold);
    int RangeSetHighThreshold(uint16_t threshold);
    int AlsSetLowThreshold(uint16_t threshold);	
    int AlsSetHighThreshold(uint16_t threshold);
    int GetRangeError(measure_data_t *Data, VL6180X_RangeData_t RangeData);
    int GetAlsError(measure_data_t *Data, VL6180X_AlsData_t AlsData);
    int RangeMeasPollSingleShot();
    int AlsMeasPollSingleShot();		
    int RangeMeasPollContinuousMode();	
    int AlsMeasPollContinuousMode();
    int AlsGetMeasurementIfReady(VL6180XDev_t dev, VL6180X_AlsData_t *pAlsData);
    int RangeMeasIntContinuousMode(void (*fptr)(void));
    int AlsMeasIntContinuousMode(void (*fptr)(void));
    int InterleavedMode(void (*fptr)(void));
    int StartInterleavedMode();
    int AlsGetThresholds(VL6180XDev_t dev, lux_t *low, lux_t *high);

	/* IO _device */
    DevI2C *_dev_i2c;
    /* Digital out pin */
    DigitalOut *_gpio0;
    /* GPIO expander */
    STMPE1600DigiOut *_expgpio0;
    /* Measure detection IRQ */
    InterruptIn *_gpio1Int;
    /* _device data */
    MyVL6180Dev_t _my_device;
    VL6180XDev_t _device;  
};

#endif // __VL6180X_CLASS_H
