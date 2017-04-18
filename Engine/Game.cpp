/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mouse.h"
#include <random>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	field(10)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (!isGameOver) {
		while (!wnd.mouse.IsEmpty()) {
			const Mouse::Event event = wnd.mouse.Read();
			auto mousePos = event.GetPos();
			if (event.GetType() == Mouse::Event::Type::LPress) {
				if (field.GetRect().Contains(Vei2(mousePos.first, mousePos.second))) {
					isGameOver = field.OnRevealedClick(Vei2(mousePos.first, mousePos.second));
				}
			}

			if (event.GetType() == Mouse::Event::Type::RPress) {
				if (field.GetRect().Contains(Vei2(mousePos.first, mousePos.second))) {
					isGameOver = field.OnFlagClick(Vei2(mousePos.first, mousePos.second));
				}
			}
		}
	}
}

void Game::ComposeFrame()
{
	field.Draw(gfx);
}
