#include "svalboard.h"
#include "eeconfig.h"

saved_values_t global_saved_values;

void write_eeprom_kb(void) {
    eeconfig_update_kb_datablock(&global_saved_values);
}

void read_eeprom_kb(void) {
    bool modified = false;
    eeconfig_read_kb_datablock(&global_saved_values);
    if (global_saved_values.version < 1) {
        global_saved_values.version = 1;
        global_saved_values.left_scroll = true;
        global_saved_values.right_dpi_index=2;
        global_saved_values.left_dpi_index=2;
        modified = true;
    }
    // As we add versions, just append here.
    if (modified) {
        write_eeprom_kb();
    }
}

const uint16_t dpi_choices[] = { 200, 400, 800, 1200, 1600, 2400 }; // If we need more, add them.
#define DPI_CHOICES_LENGTH (sizeof(dpi_choices)/sizeof(dpi_choices[0]))

void increase_left_dpi(void) {
    if (global_saved_values.left_dpi_index + 1 < DPI_CHOICES_LENGTH) {
        global_saved_values.left_dpi_index++;
        set_left_dpi(global_saved_values.left_dpi_index);
        write_eeprom_kb();
    }
}

void decrease_left_dpi(void) {
    if (global_saved_values.left_dpi_index > 0) {
        global_saved_values.left_dpi_index--;
        set_left_dpi(global_saved_values.left_dpi_index);
        write_eeprom_kb();
    }
}

void increase_right_dpi(void) {
    if (global_saved_values.right_dpi_index + 1 < DPI_CHOICES_LENGTH) {
        global_saved_values.right_dpi_index++;
        set_right_dpi(global_saved_values.right_dpi_index);
        write_eeprom_kb();
    }
}

void decrease_right_dpi(void) {
    if (global_saved_values.right_dpi_index > 0) {
        global_saved_values.right_dpi_index--;
        set_right_dpi(global_saved_values.right_dpi_index);
        write_eeprom_kb();
    }
}
// TODO: Still need to add code to save values.
void set_left_dpi(uint8_t index) {
    uprintf("LDPI: %d %d\n", index, dpi_choices[index]);
    pointing_device_set_cpi_on_side(true, dpi_choices[index]);
}

void set_right_dpi(uint8_t index) {
    uprintf("RDPI: %d %d\n", index, dpi_choices[index]);
    pointing_device_set_cpi_on_side(false, dpi_choices[index]);
}

void pointing_device_init_kb(void) {
    read_eeprom_kb();
    set_left_dpi(global_saved_values.left_dpi_index);
    set_right_dpi(global_saved_values.right_dpi_index);
}

void keyboard_post_init_kb(void) {
    pointing_device_init_kb();
    keyboard_post_init_user();
}

#ifndef SVALBOARD_REENABLE_BOOTMAGIC_LITE
// This is to override `bootmagic_lite` feature (see docs/feature_bootmagic.md),
// which can't be turned off in the usual way (via info.json) because setting
// `VIA_ENABLE` forces `BOOTMAGIC_ENABLE` in `builddefs/common_features.mk`.
//
// Obviously if you find this feature useful, you might want to define the
// SVALBOARD_... gating macro, and then possibly also (re-)define the
// `"bootmagic": { "matrix": [X,Y] },` in `info.json` to point the matrix at
// a more useful key than the [0,0] default. Ideally a center key, which is
// normally ~always present. Because the default (thumb knuckle) means that
// if you boot with the key pulled out, the eeprom gets cleared.

void bootmagic_lite(void) {
  // boo!
}
#endif

#ifdef PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#include "ps2.h"
#endif

#ifdef PS2_MOUSE_ENABLE
void ps2_mouse_init_user() {
  //sensitivity command in fourth byte 00 = 0 multiplier, FF = 2.0 multiplier
  /*
    PS2_MOUSE_SEND(0xE2);
    PS2_MOUSE_SEND(0x81);
    PS2_MOUSE_SEND(0x4A);
    PS2_MOUSE_SEND(0x59);
    */

//Z tap -- doesn't seem to work.  0x01 is on.

  PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
  PS2_MOUSE_SEND(0x47, "pts: 0x47");
  PS2_MOUSE_SEND(0x2C, "pts: 0x2C");
  PS2_MOUSE_SEND(0x00, "pts: 0x00");  // 01 turns on Z tap, 00 turns off


/*
E2 80 63
E2 81 63 XX
Drift Threshold [drift]
This parameter is used to set the threshold at which drift calibration will occur. Low drift
thresholds should be used to maximize the effectiveness of the drift calibration algorithms.
If the difference between the smoothed conversion value and the trial value remains within
the internal drift threshold, drift calibration will occur. The format of this parameter is
x"FF"- drift threshold. The default value for drift is x"FE", corresponding to a drift
threshold of 1. A reset (x"FF") or set default (x"F6") command will not affect this byte.

*/
  PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
  PS2_MOUSE_SEND(0x81, "pts: 0x81");
  PS2_MOUSE_SEND(0x63, "pts: 0x63");
  PS2_MOUSE_SEND(0x80, "pts: 0x80");  // 0xFE is default threshold of 1  try low and see what happens

/*
E2 80 64
E2 81 64 XX
XY running average drift time constant [xydriftavg]
This parameter sets the time constant used for the X and Y axis running average routines
used in conjunction with a high sensitivity TrackPoint circuit for drift control only. This
directly affects the performance of the drift cancellation algorithms. The least possible
averaging which allows calibration to take place should be used, otherwise improper
calibrations will occur during slow cursor movement. The formula used for running
average is: A = A + (N - A) X where A = running average, N = present sample (lowx or
256
lowy, low order l counts), and X is xydriftavg. The default value for xydriftavg will
depend upon the presence of a jumper between pins 26 and 21 of the TPM754. If the
jumper is present (noisier sensor), the default value will be x"40" (decimal 64)
corresponding to approximately a 4 sample average, otherwise the default value is x"80"
(decimal 128) corresponding to a 2 sample average. This value is not affected by a reset
(x"FF") or set default (x"F6") command
*/
  PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
  PS2_MOUSE_SEND(0x81, "pts: 0x81");
  PS2_MOUSE_SEND(0x63, "pts: 0x64");
  PS2_MOUSE_SEND(0x80, "pts: 0x80");  // 0x80 is default threshold of 2 samples try low and see what happens
/*

Timing on the double tap on the trackpoint:

E2 80 5E
E2 81 5E XX

Z Time Constant [zTc]
 This PtS parameter is the time constant of the running average of Z force.  Its units are in
 force sampling time units as specified in the Set Sampling Rate command on page 14.
 This determines how sharp a press has to be to be recognized.  The value of this parameter
 is 256/n, where n is the desired time constant.  The running average is then calculated with
 the formula:
  where A = running average, N = present sample, and X is
 A=A+(N−A) X
 256
 zTc.  The default value for zTc is x"26" (decimal 38) corresponding to a time constant of
 6.7 samples.  This value is not affected by a reset (x"FF") or set default (x"F6") command

*/


    PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
    PS2_MOUSE_SEND(0x81, "pts: 0x81");
    PS2_MOUSE_SEND(0x5E, "pts: 0x5E");
    PS2_MOUSE_SEND(0x00, "pts: 0x00");


  // transfer function plateau speed -- default is 0x61
/*
  PS2_MOUSE_SEND(0xE2, "tfups: 0xE2");
  PS2_MOUSE_SEND(0x80, "tfups: 0x80");
  PS2_MOUSE_SEND(0x60, "tfups: 0x60");
  PS2_MOUSE_SEND(0xf0, "tfups: 0xf0");
  */



}
#endif

