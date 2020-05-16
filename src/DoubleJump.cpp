#include "DoubleJump.h"
#include "Adventurer.h"

void DoubleJump::Enable(Adventurer& adventurer)
{
    adventurer.m_totalExtrasJump = 1;
}

void DoubleJump::Disable(Adventurer& adventurer)
{
    adventurer.m_extrasJumpCount = 0;
}
