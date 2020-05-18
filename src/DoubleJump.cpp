#include "DoubleJump.h"
#include "Adventurer.h"

void DoubleJump::Enable(Adventurer& adventurer)
{
    adventurer.m_totalExtraJumps = 1;
}

void DoubleJump::Disable(Adventurer& adventurer)
{
    adventurer.m_extraJumps = 0;
}
