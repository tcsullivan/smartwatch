/**************************************************************************/
/*!
    @file     AdaCallback.h
    @author   hathach (tinyusb.org)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2018, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef ADACALLBACK_H_
#define ADACALLBACK_H_

#include "common_inc.h"

#define CFG_CALLBACK_TASK_STACKSIZE     (512*2)
#define CFG_CALLBACK_QUEUE_LENGTH       20
#define CFG_CALLBACK_TIMEOUT            100

//#ifdef __cplusplus
//extern "C"{
//#endif

typedef struct
{
  void*   malloced_data;
  void*   callback_func;

  uint8_t arg_count;
  bool    from_isr;
//  uint8_t callback_type;
//  uint8_t _reserved[2];

  uint32_t arguments[1]; // flexible array holder
}ada_callback_t;

VERIFY_STATIC( sizeof(ada_callback_t) == 16 );

/*------------- Defer callback type, determined by number of arguments -------------*/
typedef void (*adacb_0arg_t) (void);
typedef void (*adacb_1arg_t) (uint32_t);
typedef void (*adacb_2arg_t) (uint32_t, uint32_t);
typedef void (*adacb_3arg_t) (uint32_t, uint32_t, uint32_t);
typedef void (*adacb_4arg_t) (uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*adacb_5arg_t) (uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

#ifdef __cplusplus
template<typename Func, typename... Args>
inline void _cb_setup(bool _from_isr, void *_malloced, Func _func, Args... args)
{
      uint8_t const _count = sizeof...(args);
      ada_callback_t* cb_data = (ada_callback_t*) rtos_malloc( sizeof(ada_callback_t) + (_count ? (_count-1)*4 : 0) );
      cb_data->malloced_data = _malloced;
      cb_data->callback_func = (void*)_func;
      cb_data->arg_count = _count;
      if ( _count ) {
        uint32_t arguments[] = { ((uint32_t)args)... };
        memcpy(cb_data->arguments, arguments, 4*_count);
      }

      extern void ada_callback_queue(ada_callback_t* cb_data, bool from_isr);
      ada_callback_queue(cb_data, _from_isr);
}
#endif // __cplusplus

/**
 * Schedule an function and parameters to be invoked in Ada Callback Task
 * Macro can take at least 2 and at max 7 arguments
 * - 1st arg     : pointer data that need to be freed with free(pointer) after function is invoked
 * - 2nd arg     : function to be invoked
 * - 3rd-7th arg : function argument, will be cast to uint32_t
 */
#define ada_callback(... )           _cb_setup(false, __VA_ARGS__)

/**
 * Similar to ada_callback() but invoke in ISR-context
 */
#define ada_callback_fromISR(... )   _cb_setup(true , __VA_ARGS__)

void ada_callback_init(void);
void ada_callback_queue(ada_callback_t* cb_data, bool from_isr);

//#ifdef __cplusplus
//}
//#endif

#endif /* ADACALLBACK_H_ */
