#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H
#include "Layer.h"
#include "ListMenu.h"
class PauseMenu : public Layer
{
  public:
    static PauseMenu* create();

	void update(float deltaTime) override;
	void render() override;


  private:
    PauseMenu();
	bool init();

	ListMenu* m_listMenu;	
};
#endif // PAUSE_MENU_H
