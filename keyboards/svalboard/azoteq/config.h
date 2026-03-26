#pragma once

#define I2C_DRIVER I2CD0
#define I2C1_SDA_PIN GP16
#define I2C1_SCL_PIN GP17

#define AZOTEQ_IQS5XX_TPS43
#define AZOTEQ_IQS5XX_TIMEOUT_MS 10
// Keep press_and_hold enabled in hardware to avoid gesture engine stalls,
// but don't act on it — see AZOTEQ_IQS5XX_PRESS_AND_HOLD_AS_CLICK below.
#define AZOTEQ_IQS5XX_PRESS_AND_HOLD_ENABLE true
#define AZOTEQ_IQS5XX_PRESS_AND_HOLD_AS_CLICK false

#define AZOTEQ_IQS5XX_TAP_DRAG_ENABLE true
#define AZOTEQ_IQS5XX_TAP_DRAG_WINDOW_MS 200

#define AZOTEQ_IQS5XX_SCROLL_DIVISOR 8

#define AZOTEQ_IQS5XX_TAP_TIME 200
// TAP_TIME default is 150ms. Raised since we no longer act on press_and_hold,
// so there's no need to keep TAP_TIME short to disambiguate the two.
#define AZOTEQ_IQS5XX_HOLD_TIME 300
// HOLD_TIME default is 300ms

//#define POINTING_DEVICE_MOTION_PIN GP18

#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_COMBINED
#define POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE
