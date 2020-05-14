#include "DoubleJump.h"
#include "Adventurer.h"

void DoubleJump::enable(Adventurer& adventurer)
{
    adventurer.m_totalExtrasJump = 1;
}

void DoubleJump::disable(Adventurer& adventurer)
{
    adventurer.m_extrasJumpCount = 0;
}
