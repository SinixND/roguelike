#include "Shadow.h"

Shadow::Shadow(Vector2I const& octantPosition)
{
    setSlopeLeft(octantPosition);
    setSlopeRight(octantPosition);
}

void Shadow::setSlopeLeft(Vector2I const& octantPosition)
{
    slopeLeft_ = (octantPosition.y + 0.5f) / (octantPosition.x - 0.5f);
}

void Shadow::setSlopeLeft(float slopeLeft)
{
    slopeLeft_ = slopeLeft;
}

void Shadow::setSlopeRight(Vector2I const& octantPosition)
{
    // slopeRight_ = (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f);
    slopeRight_ = std::max(EPSILON, (octantPosition.y - 0.5f) / (octantPosition.x + 0.5f));
}

void Shadow::setSlopeRight(float slopeRight)
{
    slopeRight_ = slopeRight;
}

float Shadow::getLeftAtTop(Vector2I const& octantPosition)
{
    return (octantPosition.y + 0.5f) / slopeLeft_;
}

float Shadow::getLeftAtBottom(Vector2I const& octantPosition)
{
    return (octantPosition.y - 0.5f) / slopeLeft_;
}

float Shadow::getLeft(int octantPositionHeight)
{
    // NOTE: x = y / m
    return (octantPositionHeight) / slopeLeft_;
}

float Shadow::getRightAtTop(Vector2I const& octantPosition)
{
    return (octantPosition.y + 0.5f) / slopeRight_;
}

float Shadow::getRightAtBottom(Vector2I const& octantPosition)
{
    return (octantPosition.y - 0.5f) / slopeRight_;
}

float Shadow::getRight(int octantPositionHeight)
{
    // NOTE: x = y / m
    return (octantPositionHeight) / slopeRight_;
}
