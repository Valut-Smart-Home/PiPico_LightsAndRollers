#ifndef _configuration_h
#define _configuration_h

#include <stdbool.h>
#include <stdint.h>

enum btn_click_mode {
  btn_click_None,
  btn_click_Switch,
  btn_click_Start_Stop,
};

enum btn_press_mode {
  btn_press_None,
  btn_press_OnWhenPressed,
  btn_press_Dimmer,
};

enum btn_state_read_mode {
  btn_state_read_AlwaysZero,
  btn_state_read_AlwaysMax,
  btn_state_read_Max,
  btn_state_read_Min,
  btn_state_read_Avg,
  btn_state_read_Xor,
  btn_state_read_Xor_Binary,
};

enum virtual_output_mode {
  virtual_output_None,
  virtual_output_Single,
  virtual_output_DirSwitch,
  virtual_output_Dual,
  virtual_output_ExternalPwm,
};

enum device_features {
  device_feature_fram = 0x01,
};

struct button_click_configuration {
  enum btn_click_mode clickMode;
  uint16_t pwmAffectedMask;
  uint32_t virtualOutputAffectedMask;
  uint16_t min_value;
  uint16_t max_value;
  uint32_t optionalWriteValue;
  enum btn_state_read_mode readMode;
  uint32_t pwmReadMask;
  uint32_t virtualOutputReadMask;
  uint32_t optionalReadValue;
};

struct button_press_configuration {
  enum btn_press_mode pressMode;
  uint32_t pwmAffectedMask;
  uint32_t virtualOutputAffectedMask;
  uint16_t min_value;
  uint16_t max_value;
  uint32_t optionalWriteValue;
  enum btn_state_read_mode readMode;
  uint32_t pwmReadMask;
  uint32_t virtualOutputReadMask;
  uint32_t optionalReadValue;
};

struct device_configuration {
  uint8_t slaveId;
  enum device_features features;
};

struct button_configuration {
  struct button_click_configuration firstClick;
  struct button_click_configuration secondClick;
  struct button_press_configuration press;
};

struct virtual_output_configuration {
  enum virtual_output_mode outputMode;
  union {
    uint32_t externalFirstOutputMask;
    uint16_t externalPwmAddress;
  };
  union {
    uint32_t externalSecondOutputMask;
    uint16_t externalPwmStartValue;
  };
  uint32_t delayMs;
};

#endif