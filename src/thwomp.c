#include "thwomp.h"
#include "cv64.h"

// Entrypoint function. Executed every frame as long as the thwomp is active
void thwomp_entrypoint(thwomp* self) {
    self->timer++;

    self->header.functionInfo_ID++;
    thwomp_functions[self->header.current_function[self->header.functionInfo_ID].function](self);
    self->header.functionInfo_ID--;
}

// Initialization function. Only executed once
void thwomp_init(thwomp* self) {
    atari_base_work* atari_base;

    model_info* (*ptr_modelInfo_createRootNode)(u32, void*) = modelInfo_createRootNode;
    atari_base_work* (*ptr_atariBaseWork_create)(model_info*) = atariBaseWork_create;
    atari_data_work* (*ptr_atariDataWork_create)(model_info*, u16) = atariDataWork_create;
    u32 (*ptr_atariBaseWork_attachCollider)(atari_base_work*, atari_data_work*, u32) = atariBaseWork_attachCollider;
    void (*ptr_goToNextFunc)(s8*, s16*) = goToNextFunc;

    // Initialize the timer used for timing the movements of the Thwomp
    self->timer = 0;
    
    // Create the struct that will handle the Thwomp's 3D model
    self->model = ptr_modelInfo_createRootNode(4, ptr_array_8018CDE0[0]);
    self->model->assets_file_ID = THWOMP_ASSETS_FILE;                // Thwomp's assets file ID in the Nisitenma-Ichigo table
    self->model->display_list_address = thwomp_seg5_dl_0500B750;     // Thwomp's main display list segment address

    // Initialize Thwomp's position
    // The position will be taken from the actor settings definition inside the map file
    // THe position should be the ground's position, since the Thwomp will begin in the "rising up from ground" state
    self->model->position.x = self->settings->position.x;
    self->model->position.y = self->settings->position.y;
    self->model->position.z = self->settings->position.z;

    // Initialize moving time and waiting time. These are also taken from the actor settings
    self->movingTime   = self->settings->variable_1;
    self->waitTime     = self->settings->variable_2;

    // Initialize Thwomp's size
    self->model->size.x = self->model->size.y = self->model->size.z = THWOMP_MODEL_SIZE * (f32) SIZE_MULTIPLIER;

    // Create the Thwomp's collider manager (atari_base), needed to make the collider work
    atari_base = ptr_atariBaseWork_create(self->model);
    atari_base->field_0x0A |= 0x202;        // What this does is currently unknwon, but it's needed to make the collider damageable
    
    // Create the actual collider, attach it to the manager, and set its parameters
    self->collider = ptr_atariDataWork_create(self->model, 1);
    ptr_atariBaseWork_attachCollider(atari_base, self->collider, 1);
    self->collider->damage = THWOMP_COLLIDER_DAMAGE;
    self->collider->size.x = self->collider->size.y = THWOMP_COLLIDER_SIZE;

    // Go to the first state function (thwomp_rise_up)
    ptr_goToNextFunc(self->header.current_function, &self->header.functionInfo_ID);
}

// State 0 = Thwomp is rising up from the ground
void thwomp_rise_up(thwomp* self) {
    void (*ptr_goToFunc)(u16[], s16*, s32) = goToFunc;
    
    // If we reached the top, stop rising up
    if (self->timer > self->movingTime) {
        self->timer = 0;
        ptr_goToFunc(self->header.current_function, &self->header.functionInfo_ID, THWOMP_STAY_ON_TOP);
    }
    // Else, keep rising up
    else {
        self->model->position.y += THWOMP_RISE_UP_SPEED;
    }
}

// State 1 = Thwomp is idling on top
void thwomp_stay_on_top(thwomp* self) {
    void (*ptr_goToFunc)(u16[], s16*, s32) = goToFunc;

    // Wait on top until reaching the time specified in waitTime
    // Then start falling down
    if (self->timer > self->waitTime) {
        self->timer = 0;
        ptr_goToFunc(self->header.current_function, &self->header.functionInfo_ID, THWOMP_FALL);
    }
}

// State 2 = Thwomp is falling down
void thwomp_fall(thwomp* self) {
    void (*ptr_goToFunc)(u16[], s16*, s32) = goToFunc;
    u8 (*ptr_play_sound_in_position)(s16, vec3f*) = play_sound_in_position;

    // Fall faster the more time the Thwomp keeps falling down
    self->fallingSpeed += THWOMP_FALLING_ACCELERATION;
    self->model->position.y += self->fallingSpeed;

    // When reaching the ground, stop and play a crashing sound
    if (self->model->position.y < self->settings->position.y) {
        self->model->position.y = self->settings->position.y;
        self->fallingSpeed = 0.0f;
        self->timer = 0;
        ptr_play_sound_in_position(0x105, &self->model->position);      // Sound ID 0x105 = Crashing sound
        ptr_goToFunc(self->header.current_function, &self->header.functionInfo_ID, THWOMP_HIT_GROUND);
    }
}

// State 3 = Thwomp is idling in the ground
void thwomp_hit_ground(thwomp* self) {
    void (*ptr_goToFunc)(u16[], s16*, s32) = goToFunc;
    
    // Wait on the ground until reaching the time specified in waitTime
    // Then start rising up, starting the cycle again
    if (self->timer >= self->waitTime) {
        self->timer = 0;
        ptr_goToFunc(self->header.current_function, &self->header.functionInfo_ID, THWOMP_RISE_UP);
    }
}
