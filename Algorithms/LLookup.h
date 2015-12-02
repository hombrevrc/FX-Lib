//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "LTraits.h"

namespace Sal
{
	template<typename Key, typename Numeric, typename Provider = LTraits<Key, Numeric> > class LLookup
	{
	public:
		typedef Key value_type;
		typedef Key* pointer;
		typedef const Key* const_pointer;
		typedef Key& reference;
		typedef const Key& const_reference;
		typedef const Key* iterator;
		typedef const Key* const_iterator;

	public:
		template<typename Iterator> LLookup(Iterator first, Iterator last, const double factor = 1) :
			m_provider(), m_min(), m_max(), m_begin(), m_end(), m_data(first, last)
		{
			Build(factor);
		}

		template<typename Iterator> LLookup(Iterator first, Iterator last, const Provider& provider, const double factor = 1) :
			m_provider(provider), m_min(), m_max(), m_begin(), m_end(), m_data(first, last)
		{
			Build(factor);
		}

		LLookup(const LLookup& arg) : m_provider(arg.m_provider), m_min(), m_max(), m_step(), m_begin(), m_end(), m_data(arg.m_data)
		{
			Build(arg.m_index.size());
		}

		LLookup& operator = (const LLookup& arg)
		{
			if (this != &arg)
			{
				m_provider = arg.m_provider;
				m_data = arg.m_data;
				Build(arg.m_index.size());
			}
			return *this;
		}

		const_iterator lower_bound(const Key& key) const
		{
			Numeric value = m_provider(key);

			if (value > m_max)
			{
				return end();
			}

			if (value < m_min)
			{
				value = m_min;
			}

			const size_t index = IndexFromValue(value);
			const pair<const_iterator, const_iterator>& entry = m_index[index];
			const_iterator result = std::lower_bound(entry.first, entry.second, key, m_provider);
			return result;
		}

		const_iterator upper_bound(const Key& key) const
		{
			Numeric value = m_provider(key);

			if (value < m_min)
			{
				return begin();
			}

			if (value > m_max)
			{
				value = m_max;
			}

			const size_t index = IndexFromValue(value);
			const pair<const_iterator, const_iterator>& entry = m_index[index];
			const_iterator result = std::upper_bound(entry.first, entry.second, key, m_provider);
			return result;
		}

		const_iterator find(const Key& key) const
		{
			const_iterator it = lower_bound(key);
			const bool isLess = m_provider(key, *it);
			if (isLess)
			{
				return end();
			}
			return it;
		}

	public:
		const_iterator begin() const
		{
			return m_begin;
		}

		const_iterator end() const
		{
			return m_end;
		}

		size_t size() const
		{
			return m_data.size();
		}

	private:
		void Build(const double factor)
		{
			size_t sizeOfIndex = static_cast<size_t>(m_data.size() * factor);
			if (0 == sizeOfIndex)
			{
				sizeOfIndex = 1;
			}
			Build(sizeOfIndex);
		}

		void Build(const size_t sizeOfIndex)
		{
			assert(sizeOfIndex > 0);

			std::stable_sort(m_data.begin(), m_data.end(), m_provider);

			if (!m_data.empty())
			{
				m_begin = &m_data.front();
				m_end = m_begin + m_data.size();
			}
			else
			{
				m_begin = nullptr;
				m_end = nullptr;
			}

			if (!m_data.empty())
			{
				m_min = m_provider(m_data.front());
				m_max = m_provider(m_data.back());
			}
			else
			{
				m_min = Numeric();
				m_max = Numeric();
			}
			assert(m_min <= m_max);

			m_delta = m_max - m_min;
			assert(m_delta >= 0);

			if (m_delta <= 0)
			{
				m_delta = 1;
			}

			m_index.clear();
			m_index.insert(m_index.end(), sizeOfIndex, std::make_pair<iterator, iterator>(nullptr, nullptr));
			if (!m_data.empty())
			{
				DoBuild();
			}
		}

		void DoBuild()
		{
			size_t previous = 0;
			for (const auto& element : m_data)
			{
				size_t next = IndexFromValue(element);
				assert(previous <= 1 + next);

				for (size_t index = previous; index <= next; ++index)
				{
					m_index[index].first = &element;
				}

				previous = 1 + next;
			}

			const size_t sizeOfIndex = m_index.size();
			assert(previous >= sizeOfIndex);

			for (size_t index = 1; index < sizeOfIndex; ++index)
			{
				m_index[index - 1].second = m_index[index].first;
			}

			m_index.back().second = end();
		}

		size_t IndexFromValue(const Numeric& value) const
		{
			assert(value <= m_max);
			assert(value >= m_min);
			assert(!m_index.empty());

			const size_t threshold = m_index.size() - 1;
			size_t result = static_cast<size_t>((value - m_min) * threshold / m_delta);

			if (result > threshold)
			{
				result = threshold;
			}

			return result;
		}

	private:
		Provider m_provider;
		Numeric m_factor;
		Numeric m_min;
		Numeric m_max;
		/// <summary>
		/// m_delta = (m_max - m_min) or 1, if m_max <= m_min
		/// </summary>
		Numeric m_delta;
		const_iterator m_begin;
		const_iterator m_end;
		std::vector<Key> m_data;
		std::vector< std::pair<iterator, iterator> > m_index;
	};
}
