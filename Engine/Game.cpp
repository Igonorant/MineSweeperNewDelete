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
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	menu( { gfx.GetRect().GetCenter().x,200 } )
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
	while( !wnd.mouse.IsEmpty() )
	{
		const auto e = wnd.mouse.Read();
		if( state == State::Memesweeper )
		{
			if( field->GetState() == MemeField::State::Memeing )
			{
				if( e.GetType() == Mouse::Event::Type::LPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( field->GetRect().Contains( mousePos ) )
					{
						field->OnRevealClick( mousePos );
					}
				}
				else if( e.GetType() == Mouse::Event::Type::RPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( field->GetRect().Contains( mousePos ) )
					{
						field->OnFlagClick( mousePos );
					}
				}

				if (field->GetState() == MemeField::State::Lost || field->GetState() == MemeField::State::Winrar) {
					state = State::SelectionMenu;
					delete field;
				}
			}
		}
		else
		{
			const SelectionMenu::Size s = menu.ProcessMouse( e );
			switch( s )
			{
			case SelectionMenu::Size::Small:
				field = new MemeField;
				field->Initilize(gfx.GetRect().GetCenter(),10, 10, 8);
				state = State::Memesweeper;
				break;
			case SelectionMenu::Size::Medium:
				field = new MemeField;
				field->Initilize(gfx.GetRect().GetCenter(), 25, 25, 18);
				state = State::Memesweeper;
				break;
			case SelectionMenu::Size::Large:
				field = new MemeField;
				field->Initilize(gfx.GetRect().GetCenter(), 60, 35, 25);
				state = State::Memesweeper;
				break;
			}
		}
	}
}

void Game::ComposeFrame()
{
	if( state == State::Memesweeper )
	{
		field->Draw( gfx );
		if( field->GetState() == MemeField::State::Winrar )
		{
			SpriteCodex::DrawWin( gfx.GetRect().GetCenter(),gfx );
		}
	}
	else
	{
		menu.Draw( gfx );
	}
}
