//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once
#include "LTraits.h"
#include "Nullable.h"


namespace Sal
{
	namespace Internal
	{
		template<typename TKey, typename TValue> class LMapEntry : public std::pair<TKey, TValue>
		{
		public:
			uint32_t Index;

		public:
			LMapEntry(const TKey& key) : std::pair<TKey, TValue>(key, TValue()), Index()
			{
			}

		public:
			LMapEntry(const TKey& key, const TValue& value) : std::pair<TKey, TValue>(key, value), Index()
			{
			}
		};
	}
}

namespace Sal
{
	namespace Internal
	{
		template<typename TContainer, typename TKey, typename TValue> class LMapConstIterator
		{
		public:
			LMapConstIterator() : m_pOwner(nullptr), m_pCurrent(nullptr)
			{
			}

			LMapConstIterator(const TContainer* pOwner, const uint32_t key) : m_pOwner(pOwner), m_key(key)
			{
			}

		public:
			LMapConstIterator& operator ++ ()
			{
				m_key = m_pOwner->GetNextKey(m_key);
				return *this;
			}

			LMapConstIterator operator ++ (int)
			{
				const_iterator result = *this;
				++(*this);
				return result;
			}

			LMapConstIterator& operator -- ()
			{
				m_key = m_pOwner->GetPreviousKey(m_key);
				return *this;
			}

			LMapConstIterator operator -- (int)
			{
				const_iterator result = *this;
				++(*this);
				return result;
			}

		public:
			bool operator == (const LMapConstIterator& it)
			{
				assert(m_pOwner == it.m_pOwner);
				return (m_key == it.m_key);
			}

			bool operator != (const LMapConstIterator& it)
			{
				assert(m_pOwner == it.m_pOwner);
				return (m_key != it.m_key);
			}

		public:
			const std::pair<const TKey, const TValue>* operator -> () const
			{
				const std::pair<const TKey, const TValue>* result = m_pOwner->GetEntry(m_key);
				return result;
			}

		protected:
			const TContainer* m_pOwner;
			uint32_t m_key;
		};


		template<typename TContainer, typename TKey, typename TValue> class LMapIterator : public LMapConstIterator<TContainer, TKey, TValue>
		{
		public:
			LMapIterator()
			{
			}

			LMapIterator(TContainer* pOwner, const uint32_t key) : LMapConstIterator(pOwner, key)
			{
			}

		public:
			LMapIterator& operator ++ ()
			{
				LMapConstIterator& it = *this;
				++it;
				return *this;
			}

			LMapIterator operator ++ (int)
			{
				LMapIterator result = *this;
				++(*this);
				return result;
			}

			LMapIterator& operator -- ()
			{
				LMapConstIterator& it = *this;
				--it;
				return *this;
			}

			LMapIterator operator -- (int)
			{
				LMapIterator result = *this;
				--(*this);
				return result;
			}

		public:
			const std::pair<const TKey, const TValue>* operator -> () const
			{
				const std::pair<const TKey, const TValue>* result = reinterpret_cast<const std::pair<const TKey, const TValue>*>(m_pOwner->GetEntry(m_key));
				return result;
			}

			const std::pair<const TKey, TValue>* operator -> ()
			{
				const std::pair<const TKey, TValue>* result = reinterpret_cast<const std::pair<const TKey, TValue>*>(m_pOwner->GetEntry(m_key));
				return result;
			}
		};
	}
}

namespace Sal
{
	template<typename TKey, typename TValue, typename TProvider = LTraits<TKey, uint32_t> > class LMap
	{
	public:
		typedef TKey key_type;
		typedef TValue value_type;
		typedef uint32_t size_type;
		typedef ptrdiff_t difference_type;

	public:
		typedef Internal::LMapConstIterator<LMap<TKey, TValue, TProvider>, TKey, TValue> const_iterator;
		typedef Internal::LMapIterator<LMap<TKey, TValue, TProvider>, TKey, TValue> iterator;

	private:
		typedef Internal::LMapEntry<TKey, TValue> Entry;
		typedef Nullable<Entry> NullableEntry;

		
	public:
		LMap() : m_size()
		{
		}

	public:
		size_type size() const
		{
			return m_size;
		}

		const_iterator begin() const
		{
			if (m_index.empty())
			{
				return end();
			}
			return const_iterator(m_index, m_index.front());
		}

		const_iterator end() const
		{
			return const_iterator(this, (std::numeric_limits<uint32_t>::max)());
		}

		iterator begin()
		{
			if (m_index.empty())
			{
				return end();
			}
			return iterator(this, m_index.front());
		}

		iterator end()
		{
			return iterator(this, (std::numeric_limits<uint32_t>::max)());
		}

	public:
		size_type count(const key_type& key) const
		{
			const Entry* pEntry = do_find(key);
			if (nullptr == pEntry)
			{
				return 0;
			}
			return 1;
		}

		const_iterator find(const key_type& key) const
		{
			const Entry* pEntry = do_find(key);
			if (nullptr == pEntry)
			{
				return end();
			}
			return const_iterator(this, pEntry->Index);
		}

		iterator find(const key_type& key)
		{
			Entry* pEntry = do_find(key);
			return iterator(m_index, pEntry);
		}

	private:
		const Entry* do_find(const key_type& key) const
		{
			const size_t index = m_provider(key);
			if (1 + index > m_entries.size())
			{
				return nullptr;
			}

			const NullableEntry& nEntry = m_entries[index];
			if (!nEntry.HasValue())
			{
				return nullptr;
			}

			return &nEntry.GetValue();
		}

		Entry* do_find(const key_type& key)
		{
			const LMap* pConstThis = this;
			Entry* result = const_cast<Entry*>(pConstThis->do_find(key));
			return result;
		}

	public:
		void reserve(const size_type capacity)
		{
			m_entries.reserve(capacity);
			m_index.reserve(capacity);
		}

		void clear()
		{
			m_entries.clear();
			m_index.clear();
			m_size = 0;
		}

		value_type& operator [] (const key_type& key)
		{
			const uint32_t index = m_provider(key);
			ResizeIfNeeded(index);
			NullableEntry& nEntry = m_entries[index];
			if (!nEntry.HasValue())
			{
				nEntry = Entry(key);
				m_index.push_back(index);
				nEntry.GetValue().Index = static_cast<uint32_t>(m_index.size() - 1);
				++m_size;
			}
			Entry& entry = nEntry.GetValue();
			return entry.second;
		}

	private:
		uint32_t GetNextKey(const uint32_t key) const
		{
			const Entry* pEntry = GetEntry(key);

			const uint32_t index = pEntry->Index + 1;
			if (index >= m_index.size())
			{
				return (std::numeric_limits<uint32_t>::max)();
			}
			const uint32_t result = m_index[index];
			return result;
		}

		uint32_t GetPreviousKey(const uint32_t index) const
		{
			const Entry* pEntry = GetEntry(key);

			const uint32_t index = pEntry->Index - 1;
			assert(index < m_index.size());
			const uint32_t result = m_index[index];
			return result;
		}

		const Entry* GetEntry(const uint32_t key) const
		{
			const NullableEntry& nEntry = m_entries[key];
			assert(nEntry.HasValue());
			const Entry* result = &nEntry.GetValue();
			return result;
		}

	private:
		void ResizeIfNeeded(const uint32_t index)
		{
			if (1 + index > m_entries.size())
			{
				m_entries.insert(m_entries.end(), 1 + index - m_entries.size(), NullableEntry());
			}
			m_index.reserve(m_entries.capacity());
		}

	private:
		size_type m_size;
		TProvider m_provider;
		std::vector<NullableEntry> m_entries;
		std::vector<uint32_t> m_index;

	private:
		friend class iterator;
		friend class const_iterator;
	};
}
