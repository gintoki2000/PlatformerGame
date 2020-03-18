#ifndef LEVEL_H
#define LEVEL_H
#include "AssertManager.h"
#include "NTRect.h"
class Monster;
class Level
{
  public:
    ~Level();

    static Level* load(const std::string& filename);

    void render(float deltaTime);

    /// getters
    AssertManager* getAssertManager() const { return m_assertManager; }

	const NTRect& getViewport();

	//stuff
	void addMonster(Monster* monster);
	void removeMonster(Monster* monster);

  private:
    Level();
    bool init(const std::string& filename);

	/// data fields
	bool m_isPaused;
    AssertManager* m_assertManager;
};
#endif // LEVEL_H
