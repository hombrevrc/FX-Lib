//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

namespace LMapPerformance
{
	template<typename T> void GenerateData(size_t count, vector<T>& data, vector<T>& lookup)
	{
		std::mt19937 gen(0);
		std::uniform_real_distribution<double> dist(0, static_cast<double>(2 * count));

		data.reserve(count);
		lookup.reserve(count);

		for (size_t index = 0; index < count; ++index)
		{
			data.push_back(static_cast<T>(dist(gen)));
			lookup.push_back(static_cast<T>(dist(gen)));
		}
	}

	template<typename TKey, typename TValue> bool Find(const std::unordered_map<TKey, TValue>& container, const TKey& key)
	{
		auto it = container.find(key);
		const bool result = (container.end() == it);
		return result;
	}

	template<typename TKey, typename TValue> bool Find(const Sal::LMap<TKey, TValue>& container, const TKey& key)
	{
		auto it = container.find(key);
		const bool result = (container.end() == it);
		return result;
	}

	template<typename Container> double LookupSpeedTest(const uint32_t count)
	{
		std::vector<Container::key_type> data;
		std::vector<Container::key_type> lookup;

		GenerateData(count, data, lookup);

		Container container;

		for (const auto& element : data)
		{
			container[element];
		}

		const size_t repeatsNumber = 32 * 1024 * 1024 / count;

		const uint32_t start = GetTickCount();

		size_t sum = 0;

		for (size_t iteration = 0; iteration < repeatsNumber; ++iteration)
		{
			for (const auto& element : lookup)
			{
				if (Find(container, element))
				{
					++sum;
				}
			}
		}

		const uint32_t finish = GetTickCount();

		double result = (finish - start) * 1000;
		result /= (count * repeatsNumber);

		return result;
	}

	template<typename Container> double CreationSpeedTest(const uint32_t count)
	{
		std::vector<Container::key_type> data;
		std::vector<Container::key_type> lookup;

		GenerateData(count, data, lookup);

		const uint32_t start = GetTickCount();

		Container* pContainer = new Container();

		for (const auto& element : data)
		{
			(*pContainer)[element];
		}

		const uint32_t finish = GetTickCount();

		delete pContainer;
		pContainer = nullptr;

		double result = (finish - start) * 1000;
		result /= (count);

		return result;
	}

	template<typename Container> double DestroySpeedTest(const uint32_t count)
	{
		std::vector<Container::key_type> data;
		std::vector<Container::key_type> lookup;

		GenerateData(count, data, lookup);

		Container* pContainer = new Container();

		for (const auto& element : data)
		{
			(*pContainer)[element];
		}

		const uint32_t start = GetTickCount();
		delete pContainer;
		pContainer = nullptr;

		const uint32_t finish = GetTickCount();

		double result = (finish - start) * 1000;
		result /= (count);

		return result;
	}

	void LookupSpeedTest()
	{
		std::cout << "count,unordered_map,lmap" << std::endl;
		for (uint32_t count = 32; count <= 512 * 1024; count *= 2)
		{
			std::cout << count;
			std::cout << ',' << LookupSpeedTest<std::unordered_map<uint32_t, uint32_t> >(count);
			std::cout << ',' << LookupSpeedTest<Sal::LMap<uint32_t, uint32_t> >(count);
			std::cout << std::endl;
		}
	}

	void CreationSpeedTest()
	{
		std::cout << "count,unordered_map,lmap" << std::endl;
		for (uint32_t count = 32; count <= 512 * 1024; count *= 2)
		{
			std::cout << count;
			std::cout << ',' << CreationSpeedTest<std::unordered_map<uint32_t, uint32_t> >(count);
			std::cout << ',' << CreationSpeedTest<Sal::LMap<uint32_t, uint32_t> >(count);
			std::cout << std::endl;
		}
	}

	void DestroySpeedTest()
	{
		std::cout << "count,unordered_map,lmap" << std::endl;
		for (uint32_t count = 32; count <= 512 * 1024; count *= 2)
		{
			std::cout << count;
			std::cout << ',' << DestroySpeedTest<std::unordered_map<uint32_t, uint32_t> >(count);
			std::cout << ',' << DestroySpeedTest<Sal::LMap<uint32_t, uint32_t> >(count);
			std::cout << std::endl;
		}
	}
}
