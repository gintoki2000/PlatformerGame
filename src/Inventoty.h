#ifndef INVENTORY_H
#define INVENTORY_H

class Technique;
class Inventory
{
  public:
    Inventory();
    ~Inventory();

    void       AddTechnique(Technique* technique);
    Technique* GetTechnique(int i);
    int        GetNumTechniques() const;

  private:
    bool Init();

    static constexpr int MAX_TECHNIQUES = 5;

    Technique* m_techinques[MAX_TECHNIQUES];
    int        m_numTechniques;
};
#endif // INVENTORY_H
