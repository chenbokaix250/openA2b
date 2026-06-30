#include "a2b_error.h"

const char *A2B_ResultToString(A2B_Result result)
{
    switch (result) {
        case A2B_SUCCESS:           return "SUCCESS";
        case A2B_ERR_GENERIC:        return "ERR_GENERIC";
        case A2B_ERR_INVALID_PARAM:  return "ERR_INVALID_PARAM";
        case A2B_ERR_NO_MEM:        return "ERR_NO_MEM";
        case A2B_ERR_TIMEOUT:        return "ERR_TIMEOUT";
        case A2B_ERR_UNSUPPORTED:    return "ERR_UNSUPPORTED";
        case A2B_ERR_I2C_NACK:       return "ERR_I2C_NACK";
        case A2B_ERR_I2C_TIMEOUT:    return "ERR_I2C_TIMEOUT";
        case A2B_ERR_I2C_BUSY:      return "ERR_I2C_BUSY";
        case A2B_ERR_NODE_NOT_FOUND: return "ERR_NODE_NOT_FOUND";
        case A2B_ERR_NODE_NO_RESPONSE: return "ERR_NODE_NO_RESPONSE";
        case A2B_ERR_REG_READ:       return "ERR_REG_READ";
        case A2B_ERR_REG_WRITE:      return "ERR_REG_WRITE";
        case A2B_ERR_REG_INVALID_ADDR: return "ERR_REG_INVALID_ADDR";
        case A2B_ERR_STATE_INVALID:  return "ERR_STATE_INVALID";
        default:                     return "ERR_UNKNOWN";
    }
}
