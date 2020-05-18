#ifndef MENU_H
#define MENU_H
#include <functional>
#include <string>
#include <vector>
class Menu
{
  public:
    typedef std::function<Void(const std::string&)> CallbackFunc;
    Menu();
    ~Menu();

    void Tick();
    void Paint();

	void SetOptions(const std::vector<std::string>& options);
	void SetCallbackFunc(const CallbackFunc& callback);

    const CallbackFunc& GetCallback();

  private:
    int                      m_positionX;
    int                      m_positionY;
    int                      m_width;
    int                      m_height;
    std::vector<std::string> m_options;
    CallbackFunc             m_callback;
};
#endif // MENU_H
