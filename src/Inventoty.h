#ifndef INVENTORY_H
#define INVENTORY_H

class Technique;
class Inventory
{
  public:
    Inventory();
    ~Inventory();

    void       addTechnique(Technique* technique);
    Technique* getTechnique(int i);
    int        getNumTechniques();

  private:
    bool init();

    static constexpr int MAX_TECHNIQUES = 5;

    Technique* m_techinques[MAX_TECHNIQUES];
    int        m_numTechniques;
};
#endif // INVENTORY_H
