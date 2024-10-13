#ifndef EVR_STM32_CAN_DRIVER_H
#define EVR_STM32_CAN_DRIVER_H

#include "main.h"
// Configuration to switch between polling or interrupt mode
typedef enum {
    CAN_MODE_INTERRUPT,
    CAN_MODE_POLLING
} CAN_ModeTypeDef;

// Function Prototypes
HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef *hcan, CAN_ModeTypeDef mode);
HAL_StatusTypeDef CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t StdId, uint8_t *data, uint8_t length);
HAL_StatusTypeDef CAN_Start(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_ReceiveMessage(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rxHeader, uint8_t *rxBuffer);  // Polling-based reception
HAL_StatusTypeDef CAN_ActivateNotification(CAN_HandleTypeDef *hcan);  // Interrupt-based reception


#endif // EVR_STM32_CAN_DRIVER_H
