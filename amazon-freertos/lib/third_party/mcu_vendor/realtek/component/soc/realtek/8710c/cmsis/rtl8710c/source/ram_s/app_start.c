/**************************************************************************//**
 * @file     app_start.c
 * @brief    The application entry function implementation. It initial the 
 *           application functions.
 * @version  V1.00
 * @date     2016-05-27
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#undef ROM_REGION

#include "cmsis.h"

extern void shell_cmd_init (void);
extern void shell_task (void);

//void app_start (void) __attribute__ ((noreturn));

#if !defined ( __CC_ARM ) && !(defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* ARM Compiler 4/5 */
// for __CC_ARM compiler, it will add a __ARM_use_no_argv symbol for every main() function, that cause linker report error 
/// default main
__weak int main (void)
{
    while (1) {
        shell_task ();
    }
    //return 0;
}
#endif

__weak void __iar_data_init3(void)
{
	
}

__weak void __iar_zero_init3(void)
{
	
}

void app_start (void)
{    
    dbg_printf ("Build @ %s, %s\r\n", __TIME__, __DATE__);
	
#if defined (__ICCARM__)
	// __iar_data_init3 replaced by __iar_cstart_call_ctors, just do c++ constructor
	//__iar_cstart_call_ctors(NULL);
#endif
	
    shell_cmd_init ();
    main();
#if defined ( __ICCARM__ )
	// for compile issue, If user never call this function, Liking fail 
	__iar_data_init3();
#endif
}


