#include <map>
#include <string>

#include <SFML/Graphics.hpp>

class FontLibrary
{
public:
    FontLibrary() = delete;
    static void initialize();
    static sf::Font& get(const std::string& name);

private:
    static void initFont(const std::string& name, const std::string& path);
    static std::map<std::string, sf::Font> fonts_;
};