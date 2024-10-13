#include "EVR_STM32_CAN_Driver.h"

// CAN Header, Rx Header, and mailbox
static CAN_TxHeaderTypeDef TxHeader;
static uint32_t TxMailbox;

static CAN_ModeTypeDef selectedMode;  // To store the user-selected mode

// CAN Initialization Function
HAL_StatusTypeDef CAN_Init(CAN_HandleTypeDef *hcan, CAN_ModeTypeDef mode)
{
    selectedMode = mode;

    // Initialize CAN Tx header
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

    // If user selects interrupt mode, enable CAN receive interrupt
    if (selectedMode == CAN_MODE_INTERRUPT)
    {
        return CAN_ActivateNotification(hcan);  // Enable interrupt reception
    }
    return HAL_OK;  // Return OK for polling
}

// Start CAN communication
HAL_StatusTypeDef CAN_Start(CAN_HandleTypeDef *hcan)
{
    return HAL_CAN_Start(hcan);  // Return HAL_OK or error status
}

// Send CAN message
HAL_StatusTypeDef CAN_SendMessage(CAN_HandleTypeDef *hcan, uint32_t StdId, uint8_t *data, uint8_t length)
{
    // Update the Standard ID and Data Length Code
    TxHeader.StdId = StdId;
    TxHeader.DLC = length;

    // Transmit the message
    return HAL_CAN_AddTxMessage(hcan, &TxHeader, data, &TxMailbox);
}

// Enable CAN interrupt for RX FIFO0 (for interrupt-based reception)
HAL_StatusTypeDef CAN_ActivateNotification(CAN_HandleTypeDef *hcan)
{
    // Activate FIFO 0 Message Pending Interrupt
    return HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

// Interrupt callback function for receiving CAN message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
//    {
//        // Process received message
////        printf("Interrupt Mode: Received CAN message with ID: 0x%X\r\n", RxHeader.StdId);
////        printf("Data: 0x%X 0x%X 0x%X 0x%X\r\n", RxData[0], RxData[1], RxData[2], RxData[3]);
//    }
//    else
//    {
//        // Error handling inside interrupt callback is optional
//        Error_Handler();
//    }
}

// Polling-based reception function
HAL_StatusTypeDef CAN_ReceiveMessage(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rxHeader, uint8_t *rxBuffer)
{
    // Only perform polling if the mode is set to polling
    if (selectedMode == CAN_MODE_POLLING)
    {
        // Check if messages are pending in FIFO0
        if (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0)
        {
            // Retrieve the message header and data into the user-provided buffer
            if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, rxHeader, rxBuffer) == HAL_OK)
            {
                // Message received successfully
                return HAL_OK;
            }
            else
            {
                // Reception error
                return HAL_ERROR;
            }
        }
        else
        {
            // No messages in FIFO, can return HAL_BUSY if needed
            return HAL_BUSY;  // Indicate that no message was available
        }
    }
    return HAL_ERROR;  // Error: wrong mode
}
