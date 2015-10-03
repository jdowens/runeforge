#include<RF/tilemap.h>

rf::TileMap::TileMap(size_t size_x, size_t size_y)
{
	m_layout = new int*[size_x];
	for (size_t i = 0; i < size_x; i++)
		m_layout[i] = new int[size_y];

	m_tileIds.push_back(sf::IntRect(0, 128, 64, 64));
	m_tileIds.push_back(sf::IntRect(0, 192, 64, 64));
	m_x = size_x;
	m_y = size_y;
}

void rf::TileMap::TileMap::setTexture(sf::Texture& texture)
{
	m_texture = texture;
}

void rf::TileMap::setTile(int id, size_t x, size_t y)
{
	m_layout[x][y] = id;
}

void rf::TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < m_x; i++)
	{
		for (size_t j = 0; j < m_y; j++)
		{
			if (m_layout[i][j] != -1)
			{
				sf::Sprite sprite;
				sprite.setTexture(m_texture);
				int id = m_layout[i][j];
				sprite.setTextureRect(m_tileIds[m_layout[i][j]]);
				sprite.setPosition(sf::Vector2f(i*64.0f, j*64.0f));
				target.draw(sprite, states);
			}
		}
	}
}