#ifndef SDK_ANIMATIONLAYER_HPP
#define SDK_ANIMATIONLAYER_HPP

class AnimationLayer
{
public:
    char pad_0000[24];
    i32 order;
    i32 sequence; // 0x1C
    float_t prevCycle;
    float_t weight;
    float_t weightDeltaRate;
    float_t playbackRate;
    float_t cycle;
    void *owner; // 0x38 // player's thisptr
    char pad_0038[8]; // 0x40
}; //Size: 0x0048

#endif // SDK_ANIMATIONLAYER_HPP
