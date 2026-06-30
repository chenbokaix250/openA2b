#include "a2b_state.h"
#include "a2b_logger.h"

/* Valid state transitions */
static bool is_valid_transition(A2B_State from, A2B_State to)
{
    switch (from) {
        case A2B_STATE_INIT:
            return (to == A2B_STATE_IDLE);
        case A2B_STATE_IDLE:
            return (to == A2B_STATE_WAIT_POWER || to == A2B_STATE_ERROR ||
                    to == A2B_STATE_SHUTDOWN);
        case A2B_STATE_WAIT_POWER:
            return (to == A2B_STATE_WAIT_PLL || to == A2B_STATE_ERROR);
        case A2B_STATE_WAIT_PLL:
            return (to == A2B_STATE_DISCOVERY || to == A2B_STATE_ERROR);
        case A2B_STATE_DISCOVERY:
            return (to == A2B_STATE_CONFIG_NODE || to == A2B_STATE_ERROR);
        case A2B_STATE_CONFIG_NODE:
            return (to == A2B_STATE_CONFIG_SLOT || to == A2B_STATE_ERROR);
        case A2B_STATE_CONFIG_SLOT:
            return (to == A2B_STATE_CONFIG_ROUTE || to == A2B_STATE_ERROR);
        case A2B_STATE_CONFIG_ROUTE:
            return (to == A2B_STATE_RUNNING || to == A2B_STATE_ERROR);
        case A2B_STATE_RUNNING:
            return (to == A2B_STATE_LINK_DOWN || to == A2B_STATE_IDLE ||
                    to == A2B_STATE_ERROR || to == A2B_STATE_SHUTDOWN);
        case A2B_STATE_LINK_DOWN:
            return (to == A2B_STATE_RECOVERY || to == A2B_STATE_IDLE ||
                    to == A2B_STATE_ERROR);
        case A2B_STATE_RECOVERY:
            return (to == A2B_STATE_RUNNING || to == A2B_STATE_IDLE ||
                    to == A2B_STATE_ERROR);
        case A2B_STATE_ERROR:
            return (to == A2B_STATE_IDLE || to == A2B_STATE_SHUTDOWN);
        case A2B_STATE_SHUTDOWN:
            return false;  /* Terminal state */
        default:
            return false;
    }
}

void A2B_StateMachineInit(A2B_StateMachine *sm, A2B_StateCallback callback, void *priv)
{
    if (!sm) return;

    sm->current_state = A2B_STATE_INIT;
    sm->callback = callback;
    sm->callback_priv = priv;
}

A2B_State A2B_StateMachineGetState(const A2B_StateMachine *sm)
{
    return sm ? sm->current_state : A2B_STATE_ERROR;
}

A2B_Result A2B_StateMachineTransition(A2B_StateMachine *sm, A2B_State new_state)
{
    if (!sm) {
        return A2B_ERR_INVALID_PARAM;
    }

    A2B_State old_state = sm->current_state;

    if (!is_valid_transition(old_state, new_state)) {
        A2B_LOG_ERROR(A2B_LOG_TAG_STATE, "Invalid transition: %d -> %d",
                       old_state, new_state);
        return A2B_ERR_STATE_INVALID;
    }

    sm->current_state = new_state;

    A2B_LOG_INFO(A2B_LOG_TAG_STATE, "State transition: %d -> %d",
                  old_state, new_state);

    if (sm->callback) {
        sm->callback(old_state, new_state, sm->callback_priv);
    }

    return A2B_SUCCESS;
}

bool A2B_StateMachineIsValidTransition(A2B_State from, A2B_State to)
{
    return is_valid_transition(from, to);
}
