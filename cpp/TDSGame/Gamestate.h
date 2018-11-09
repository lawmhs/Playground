#include <iostream>
#include <vector>
#include"Gameobjs"

class Gamestate {
public:
	Gamestate();
	
private:
	std::vector<std::shared_ptr<Gameobjs>> gameobjs;
}
