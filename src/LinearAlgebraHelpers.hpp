#include <SFML/System.hpp>

float distance(const sf::Vector2f &v1, const sf::Vector2f &v2);

float dot_product(const sf::Vector2f &v1, const sf::Vector2f &v2);

float magnitude(const sf::Vector2f &v);

sf::Vector2f unit(const sf::Vector2f &v);

sf::Vector2f rotate(const sf::Vector2f &v, float theta);

//does not check for magnitudes being zero
float angle_between(const sf::Vector2f &v1, const sf::Vector2f &v2);
