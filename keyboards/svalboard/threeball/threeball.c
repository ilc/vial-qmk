#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"
#include <math.h>

#define REAL_CPI  6000
// Technically the number below should be 80, but 100 felt better.
#define SCROLL_DIVISOR_MULT 100

// This is defined in the number of sensor reports we get to see, before making a choice.
#define CHOICE_TIME 20

// This how long the ball has to stop to be considered IDLE.
#define IDLE_TIME 200

enum states {
    IDLE,
    SCROLL_V,
    NORMAL
};

static int current_state = IDLE;

extern const pointing_device_driver_t real_device_driver;
uint16_t user_cpi = 0;
int16_t divisor = 0;
int scroll_divisor = 0;


int _dx;
int _dy;
int _dv;

#define SQRT2_2 0.707107f

report_mouse_t handle_normal(pmw33xx_report_t report0, pmw33xx_report_t report1) {
    report_mouse_t mouse_report;

    _dx += (report0.delta_y - report1.delta_y);
    _dy += (report0.delta_y + report1.delta_y);

    mouse_report.x = _dx / divisor;
    _dx -= mouse_report.x * divisor;
    mouse_report.y = _dy / divisor;
    _dy -= mouse_report.y * divisor;

    //    mouse_report.x = mouse_report.x;
    
    mouse_report.h = 0;
    mouse_report.v = 0;

    return mouse_report;
}

report_mouse_t handle_scroll_v(pmw33xx_report_t report0, pmw33xx_report_t report1) {
    report_mouse_t mouse_report;

    _dv -= report1.delta_x;
    mouse_report.v = _dv / scroll_divisor;
    _dv -= mouse_report.v * scroll_divisor;
    mouse_report.h = 0;

    mouse_report.x = 0;
    mouse_report.y = 0;

    return mouse_report;
}

pmw33xx_report_t fix_report(pmw33xx_report_t report) {
    int16_t swap;

    swap = report.delta_x;
    report.delta_x = -report.delta_y;
    report.delta_y = -swap;

    return report;
}

struct actual_measure {
    int x;
    int y;
    int z;
};


// Sensor placements:
// - 0 is on the bottom, giving true X, Y.
// - 1 is looking down the Y axis pointed up at Z at 45 degrees.
pmw33xx_report_t reports0[CHOICE_TIME];
pmw33xx_report_t reports1[CHOICE_TIME];

uint16_t idle_start;
int16_t report_count = 0;
//    int distance;
bool waiting_to_idle = false;
void determine_state(pmw33xx_report_t report0, pmw33xx_report_t report1) {
//    int z = 0;
//    int z_t = 0;
//    int dx;
//    int dy;
//    int x;
//    int y;
#if 0
    // Handle delay before going into idle.
    if (report0.delta_x == 0 && report0.delta_y == 0 &&
        report1.delta_x == 0 && report0.delta_y == 0) {
        if (current_state == IDLE)
            return;
        if (waiting_to_idle && timer_elapsed(idle_start) > IDLE_TIME) {
            current_state = IDLE;
            return;
        }
        if (waiting_to_idle) {
            return;
        }
        // Do detect and flush.
        report_count = 0;
        idle_start = timer_read();
        waiting_to_idle = true;
        return;
    }
#endif
//    x = report1.delta_x;
//    y = report1.delta_y;
    // we decided, we have not idled out.  Same mode.
//    if (report_count == -1) {
//        return;
//    }
//    reports0[report_count] = report0;
//    reports1[report_count] = report1;
//    report_count++;
//
//    if (report_count >= CHOICE_TIME) {
//        make_a_choice();
//
//         figure out next step.
//   }


    current_state = NORMAL;
#if 0
    dx = report1.delta_x - report0.delta_x;
    dy = report1.delta_y - report0.delta_y;

    z =  (dx + dy) / 2;
    z_t = sqrt((dx * dx) + (dy * dy));
    z_t = z > 0 ? z_t : -z_t;
    uprintf("z: %d %d ", z, z_t);
    waiting_to_idle = false;

    current_state = IDLE;
#endif
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    pmw33xx_report_t report0 = pmw33xx_read_burst(0);  // Back Sensor
    pmw33xx_report_t report1 = pmw33xx_read_burst(1);  // Bottom Sensor

    //    report0 = fix_report(report0);
    //    report1 = fix_report(report1);

    determine_state(report0, report1);
//    uprintf("%3d %3d %3d %3d %4d %d\n", report0.delta_x, report0.delta_y, report1.delta_x, report1.delta_y,
//    uprintf("%3d %3d %4d %d\n", report1.delta_x, report1.delta_y,
//    (int)((report0.delta_x * report0.delta_x) + (report0.delta_y * report0.delta_y)) - (int)((report1.delta_x * report1.delta_x) + (report1.delta_y * report1.delta_y)),
//            current_state);

    switch (current_state) {
        case NORMAL:
	    mouse_report = handle_normal(report0, report1);
            break;
        case SCROLL_V:
	    mouse_report = handle_scroll_v(report0, report1);
            break;
        case IDLE:
            mouse_report.x = 0;
            mouse_report.y = 0;
            mouse_report.h = 0;
            mouse_report.v = 0;
            break;
    }


    return mouse_report;
}

void pointing_device_init_kb(void) {
    pmw33xx_init(1);         // index 1 is the second device.
    pmw33xx_set_cpi(0, REAL_CPI); // applies to first senso
    pmw33xx_set_cpi(1, REAL_CPI); // applies to second sensor
    pointing_device_init_user();
}


// We must keep and return the requested CPI even if we use a divisor
// and it doesn't come out exact.
void pointing_device_driver_set_cpi(uint16_t v) {
    divisor = REAL_CPI / v;
    scroll_divisor = divisor * SCROLL_DIVISOR_MULT;
    _dx = 0; _dy = 0; _dv = 0;
    user_cpi = v;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return user_cpi;
}
