/*
 *       Example of AC_Notify library .
 *       DIYDrones.com
 */

#include <AP_HAL/AP_HAL.h>
#include <AP_HAL_ChibiOS/Util.h>
#include <AP_HAL_ChibiOS/ToneAlarm.h>
#include <AP_Notify/AP_Notify.h>          // Notify library
#include <AP_Notify/AP_BoardLED.h>        // Board LED library
#include <AP_Notify/ToneAlarm_ChibiOS.h>

void setup();
void loop();

const AP_HAL::HAL& hal = AP_HAL::get_HAL();

// create board led object
AP_BoardLED board_led;

static NotifyDevice* _device;


void setup()
{
    hal.console->printf("AP_Notify library test\n");

    _device = new ToneAlarm_ChibiOS();
    _device->init();
    ChibiOS::Util::from(hal.util)->toneAlarm_init();

    // initialise the board leds
    board_led.init();

    // turn on initialising notification
    AP_Notify::flags.initialising = true;
    AP_Notify::flags.gps_status = 1;
    AP_Notify::flags.armed = 1;
    AP_Notify::flags.pre_arm_check = 1;
}

void loop()
{
    static int tune = 0;
    static uint32_t then = AP_HAL::millis();
    hal.scheduler->delay(20);
    // low level state machine clock
    ChibiOS::Util::from(hal.util)->_toneAlarm_timer_tick();

    uint32_t now = AP_HAL::millis();
    if ((now - then) > 2000) {
        then = now;
        tune++;
//        if (tune==1 || tune==6 || tune==7) tune++;
        if (tune >= 11) {
            tune = 0;
        }
//        hal.console->printf("playing tune %u\n", tune);
        ChibiOS::Util::from(hal.util)->toneAlarm_set_tune(tune);
    }
}

AP_HAL_MAIN();
