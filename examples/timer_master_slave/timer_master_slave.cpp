/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/dma.h"


using namespace stm32plus;


/**
 * Timer master/slave demonstration. Use one timer to switch another one on and off at
 * predefined intervals.
 *
 * Timer2 is configured as the master timer with a frequency of 2000Hz and a reload
 * value of 8000. Channel 1 is configured to generate a PWM wave with a 25% duty cycle.
 * That means 1 seconds on and 3 seconds off.
 *
 * Timer3 is configured as the slave timer with a frequency of 2000Hz and a reload value
 * of 200 with a GPIO output toggle feature so it flashes on/off 5 times per second.
 *
 * If left to run freely timer3 would flash continually at 5Hz. However, attaching it as
 * a slave to the PWM wave generated by timer2 means that it will flash for 1 second then
 * switch off for 3 seconds.
 *
 * The output is generated on PA6. You can wire that directly to a LED or a logic analyser
 * to see the results. My STM32F103ZET6 development board has a LED on PF6 so I've wired
 * PA6 to PF6 and enabled PF6 for output in this demo code.
 *
 * If you're using the F4 Discovery board then wire up PA6 to PD13. If you're using the
 * F1 VL or F0 Discovery board then wire up PA6 to PC8.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F042F6P6
 *   STM32F051R8T6
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class TimerMasterSlaveTest {

  public:

    void run() {

      /*
       * Enable PF6 for output so we can see the output on the LED that's connected there.
       * Wire up PA6 to PF6 to see the output.
       */

      GpioF<DefaultDigitalOutputFeature<6> > pf;

      /*
       * Create an instance of Timer2 (master) running off the internal clock with
       * channel 1 and master features.
       */

      Timer2<
        Timer2InternalClockFeature,         // the timer bus is APB1 (APB on the F0)
        TimerChannel1Feature<>,             // we're going to use channel 1
        TimerOutputCompare1MasterFeature    // we're the master timer using OC1 as the trigger
      > masterTimer;

      /*
       * Set the frequency of Timer2 to 2000Hz with a reload value of 8000.
       */

      masterTimer.setTimeBaseByFrequency(2000,7999);

      /*
       * Initialise channel 1's comparator for use as a PWM output with an
       * initial duty cycle of 25%
       */

      masterTimer.initCompareForPwmOutput(25);

      /*
       * Create an instance of Timer3 (slave) running off the internal clock
       * with channel 1 and unremapped GPIO output features.
       */

      Timer3<
        Timer3InternalClockFeature,       // the timer bus is APB1 (APB on the F0)
        TimerChannel1Feature<>,           // we're going to use channel 1
        Timer2MasterTimer3SlaveFeature<   // timer3 is a slave to timer2 (ITR1)
          TIM_SlaveMode_Gated             // gated mode - the slave counter goes on and off with the trigger
        >,
        Timer3GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM3_CH1_OUT                    // we will output channel 1 to GPIO
        >
      > slaveTimer;

      /*
       * Set an up-timer up to tick at 2000Hz with an auto-reload value of 200
       * The timer will count from 0 to 199 inclusive then reset back to 0.
       * It will do this 10 times per second
       *
       * Note that the lowest frequency you can set is 1098 for a 72Mhz timer clock source.
       * This is because the maximum prescaler value is 65536 (72Mhz/65536 = 1098Hz).
       */

      slaveTimer.setTimeBaseByFrequency(2000,199);

      /*
       * Initialise the channel 1 output compare value to 2000 with the default
       * action of toggle.
       */

      slaveTimer.initCompare(199);

      /*
       * Enable master feature and switch the timer on. Enabling of master feature must happen
       * after the rest of the timer has been set up (above)
       */

      masterTimer.enableMasterFeature();
      masterTimer.enablePeripheral();

      /*
       * Enable slave feature and switch the timer on. Enabling of slave feature must happen
       * after the rest of the timer has been set up (above)
       */

      slaveTimer.enableSlaveFeature();
      slaveTimer.enablePeripheral();

      /*
       * It's all running automatically now. The on/off duration of the flashing can be controlled
       * using the duty cycle of timer2.
       */

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  TimerMasterSlaveTest test;
  test.run();

  // not reached
  return 0;
}
