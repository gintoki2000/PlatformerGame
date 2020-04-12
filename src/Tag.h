#ifndef TAG_H
#define TAG_H
struct Tag
{
    int tag1;
    int tag2;
    int tag3;
};

struct ITag
{
    virtual ~ITag() {}
    virtual Tag& getTag() = 0;
};
#endif // TAG_H
