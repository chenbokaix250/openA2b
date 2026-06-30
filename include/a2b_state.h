#ifndef A2B_STATE_H
#define A2B_STATE_H

#include "a2b_types.h"
#include "a2b_error.h"

/* State change callback */
typedef void (*A2B_StateCallback)(A2B_State old_state, A2B_State new_state, void *priv);

/**
 * State machine context
 */
typedef struct _A2B_StateMachine {
    A2B_State            current_state;
    A2B_StateCallback    callback;
    void                *callback_priv;
} A2B_StateMachine;

/**
 * Initialize state machine
 */
void A2B_StateMachineInit(A2B_StateMachine *sm, A2B_StateCallback callback, void *priv);

/**
 * Get current state
 */
A2B_State A2B_StateMachineGetState(const A2B_StateMachine *sm);

/**
 * Attempt state transition
 * Returns A2B_SUCCESS if transition is valid, A2B_ERR_STATE_INVALID otherwise
 */
A2B_Result A2B_StateMachineTransition(A2B_StateMachine *sm, A2B_State new_state);

/**
 * Check if transition is valid
 */
bool A2B_StateMachineIsValidTransition(A2B_State from, A2B_State to);

#endif /* A2B_STATE_H */
