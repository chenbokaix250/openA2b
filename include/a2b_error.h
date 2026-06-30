#ifndef A2B_ERROR_H
#define A2B_ERROR_H

#include <stdint.h>

typedef int32_t A2B_Result;

/* Success */
#define A2B_SUCCESS             ((A2B_Result)0)

/* Generic errors */
#define A2B_ERR_GENERIC         ((A2B_Result)-1)
#define A2B_ERR_INVALID_PARAM   ((A2B_Result)-2)
#define A2B_ERR_NO_MEM          ((A2B_Result)-3)
#define A2B_ERR_TIMEOUT         ((A2B_Result)-4)
#define A2B_ERR_UNSUPPORTED     ((A2B_Result)-5)

/* I2C errors */
#define A2B_ERR_I2C_NACK        ((A2B_Result)-10)
#define A2B_ERR_I2C_TIMEOUT     ((A2B_Result)-11)
#define A2B_ERR_I2C_BUSY        ((A2B_Result)-12)

/* Node errors */
#define A2B_ERR_NODE_NOT_FOUND  ((A2B_Result)-20)
#define A2B_ERR_NODE_NO_RESPONSE ((A2B_Result)-21)

/* Register errors */
#define A2B_ERR_REG_READ        ((A2B_Result)-30)
#define A2B_ERR_REG_WRITE       ((A2B_Result)-31)
#define A2B_ERR_REG_INVALID_ADDR ((A2B_Result)-32)

/* State errors */
#define A2B_ERR_STATE_INVALID   ((A2B_Result)-40)

/* Convert result to string */
const char *A2B_ResultToString(A2B_Result result);

#endif /* A2B_ERROR_H */
