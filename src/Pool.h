#ifndef POOL_H
#define POOL_H

template <class TObject> class BasePool;

template <class TObject> class PoolableObject
{

  public:
    BasePool<TObject>* getPool() const { return m_pool; }

  private:
    TObject*           m_next;
    BasePool<TObject>* m_pool;

    friend class BasePool<TObject>;
};

struct IPool
{
    virtual ~IPool() {}
    virtual void* alloc()         = 0;
    virtual void  free(void* obj) = 0;
};

template <class TObject> class BasePool : public IPool
{
  public:
    ~BasePool() { delete[] m_objects; }
    static BasePool* create(int numObjects)
    {
        BasePool* ret = new BasePool;
        if (ret->init(numObjects))
        {
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void* alloc() override
    {
        SDL_assert(m_freeList != nullptr && "No object available");
        TObject* newObject = m_freeList;
        m_freeList         = m_freeList->m_next;
        --m_numFreeObjects;
        return newObject;
    }
    void free(void* object) override
    {
        SDL_assert(object >= m_objects && object < m_objects + m_numObjects);
        TObject* _object = static_cast<TObject*>(object);
        _object->m_next  = m_freeList;
        m_freeList       = _object;
        ++m_numFreeObjects;
    }

    int getNumObjects() const { return m_numObjects; }
    int getNumFreeObjects() const { return m_numFreeObjects; }

  protected:
    BasePool() { m_objects = nullptr; }
    virtual bool init(int numObjects)
    {
        m_objects = new TObject[(unsigned)numObjects];
        for (int i = 0; i < numObjects - 1; ++i)
        {
            m_objects[i].m_next = &m_objects[i + 1];
        }
        m_objects[numObjects - 1].m_next = nullptr;
        for (int i = 0; i < numObjects; ++i)
        {
            m_objects[i].m_pool = this;
        }
        m_numObjects     = numObjects;
        m_freeList       = &m_objects[0];
        m_numFreeObjects = numObjects;
        return true;
    }

    TObject* m_objects;
    TObject* m_freeList;
    int      m_numObjects;
    int      m_numFreeObjects;
};

#endif // POOL_H
