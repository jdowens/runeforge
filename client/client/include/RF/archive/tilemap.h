#ifndef _TILEMAP_H
#define _TILEMAP_H

#include<vector>
#include<SFML/Graphics.hpp>

namespace rf
{
	class TileMap : public sf::Drawable
	{
	public :
		TileMap(size_t size_x, size_t size_y);
		void setTile(int id, size_t x, size_t y);
		void setTexture(sf::Texture& texture);
	protected :
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
		std::vector<sf::IntRect> m_tileIds;
		sf::Texture m_texture;
		int** m_layout;
		size_t m_x;
		size_t m_y;
	};
}

#endif