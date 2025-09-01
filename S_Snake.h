#pragma once

#include <string>
#include <deque>
#include <cstdint> // for std::uint32_t


namespace S_Snake
{
	enum class S_SNAKE_DIRECTION
	{
		LEFT = 0, UP, RIGHT, DOWN, INVALID
	};

	struct S_COORD
	{
		S_COORD() = default;
		S_COORD(std::uint32_t x, std::uint32_t y) : m_X{ x }, m_Y{ y } {}

		~S_COORD() = default;

		std::uint32_t X() const
		{
			return m_X;
		}

		std::uint32_t Y() const
		{
			return m_Y;
		}

		void X(std::uint32_t x)
		{
			m_X = x;
		}

		void Y(std::uint32_t y)
		{
			m_Y = y;
		}

		bool operator==(S_COORD rhs) const
		{
			return (this->X() == rhs.X() && this->Y() == rhs.Y());
		}

		bool operator!=(S_COORD rhs) const
		{
			return !(*this == rhs);
		}

	public:
		std::uint32_t m_X{}, m_Y{};
	};

	class S_SNAKE
	{
	public:
		S_SNAKE()
		{
			reset();
		}

		~S_SNAKE() = default;

		void reset()
		{
			m_SNAKE.clear();
			m_SNAKE.push_front(S_COORD{ 1,1 }); // Tail
			m_SNAKE.push_front(S_COORD{ 2,1 }); // Body
			m_SNAKE.push_front(S_COORD{ 3,1 }); // Head

			m_CURRENT_DIRECTION = S_SNAKE_DIRECTION::RIGHT;
		}

		void reset(std::uint32_t startX, std::uint32_t startY, std::uint16_t length, S_SNAKE_DIRECTION initialDirection)
		{
			m_SNAKE.clear();
			m_INITIAL_LENGTH = length;
			std::int8_t dx{ 0 }, dy{ 0 };

			S_COORD coord{ startX, startY };

			if		(initialDirection == S_SNAKE_DIRECTION::LEFT)  { dx = 1;  }
			else if (initialDirection == S_SNAKE_DIRECTION::UP)    { dy = -1; }
			else if (initialDirection == S_SNAKE_DIRECTION::RIGHT) { dx = -1; }
			else if (initialDirection == S_SNAKE_DIRECTION::DOWN)  { dy = 1;  }

			for (std::uint16_t i = 0; i < length; ++i)
			{
				m_SNAKE.push_back(coord);

				coord.X(coord.X() + dx);
				coord.Y(coord.Y() + dy);
			}
		}

		void set_direction(S_SNAKE_DIRECTION direction)
		{
			if (direction == m_CURRENT_DIRECTION)
			{
				return;
			}

			if ((m_CURRENT_DIRECTION == S_SNAKE_DIRECTION::LEFT  && direction == S_SNAKE_DIRECTION::RIGHT) ||
				(m_CURRENT_DIRECTION == S_SNAKE_DIRECTION::UP    && direction == S_SNAKE_DIRECTION::DOWN)  ||
				(m_CURRENT_DIRECTION == S_SNAKE_DIRECTION::RIGHT && direction == S_SNAKE_DIRECTION::LEFT)  ||
				(m_CURRENT_DIRECTION == S_SNAKE_DIRECTION::DOWN  && direction == S_SNAKE_DIRECTION::UP))
			{
				return;
			}

			m_CURRENT_DIRECTION = direction;
		}

		S_SNAKE_DIRECTION get_direction()
		{
			return m_CURRENT_DIRECTION;
		}

		void move()
		{
			S_COORD newCoord = m_SNAKE.front();

			switch (m_CURRENT_DIRECTION)
			{
			case S_SNAKE_DIRECTION::LEFT  : newCoord.X(newCoord.X() - 1); break;
			case S_SNAKE_DIRECTION::UP    : newCoord.Y(newCoord.Y() - 1); break;
			case S_SNAKE_DIRECTION::RIGHT : newCoord.X(newCoord.X() + 1); break;
			case S_SNAKE_DIRECTION::DOWN  : newCoord.Y(newCoord.Y() + 1); break;
			}

			m_SNAKE.push_front(newCoord);
		}

		void remove_tail()
		{
			if (m_SNAKE.size() > m_INITIAL_LENGTH)
			{
				m_SNAKE.pop_back();
			}
		}

		void remove_tail(bool ateFood)
		{
			if (!ateFood && m_SNAKE.size() > m_INITIAL_LENGTH)
			{
				m_SNAKE.pop_back();
			}
		}

		S_COORD get_head() const
		{
			return m_SNAKE.front();
		}

		const std::deque<S_COORD>& get_snake() const
		{
			return m_SNAKE;
		}

		bool check_self_collision()
		{
			if (m_SNAKE.size() < 4) { return false; } // At least 4 segments are needed for snake self collision.

			S_COORD head = m_SNAKE.front();

			for (auto it = m_SNAKE.begin() + 1; it != m_SNAKE.end(); ++it)
			{
				if (*it == head) { return true; }
			}

			return false;
		}

	private:
		std::deque<S_COORD> m_SNAKE;
		S_SNAKE_DIRECTION m_CURRENT_DIRECTION;

		std::uint16_t m_INITIAL_LENGTH{ 3 };
	};
}