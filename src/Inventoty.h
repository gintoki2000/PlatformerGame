#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
class Technique;
class Item;
class Inventory
{
  public:
    Inventory();
    ~Inventory();

    void       AddTechnique(Technique* technique);
    Technique* GetTechnique(int i);
    int        GetNumTechniques() const;

    void  AddItem(Item* item);
    Item* GetItem(int i);
    int   GetNumItems() const;

  private:
    std::vector<Technique*> m_techinques;
    std::vector<Item*>      m_items;
};
#endif // INVENTORY_H
