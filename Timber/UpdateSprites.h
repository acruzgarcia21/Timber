#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

void updateBee(float&, Sprite&, bool&, const Time&);
void updateCloud(std::vector<Sprite>&, std::vector<bool>&, std::vector<float>&, const Time&);
void updateBranches(int);
