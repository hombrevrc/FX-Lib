//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

namespace LLookupPerformance
{
	template<typename T> void GenerateData(uint32_t count, vector<T>& data, vector<T>& lookup)
	{
		std::mt19937 gen(0);
		std::uniform_real_distribution<double> dist(0, count);

		data.reserve(count);
		lookup.reserve(count);

		for (size_t index = 0; index < count; ++index)
		{
			data.push_back(static_cast<T>(dist(gen)));
			lookup.push_back(static_cast<T>(dist(gen)));
		}
	}

	template<typename T> bool Find(const std::vector<T>& container, const T& key)
	{
		auto it = lower_bound(container.begin(), container.end(), key);
		if (container.end() == it)
		{
			return false;
		}
		return true;
	}

	template<typename T> bool Find(const std::multiset<T>& container, const T& key)
	{
		auto it = container.lower_bound(key);
		const bool result = (container.end() != it);
		return result;
	}

	template<typename T> bool Find(const Sal::LLookup<T, T>& container, const T& key)
	{
		auto it = container.lower_bound(key);
		const bool result = (container.end() != it);
		return result;
	}

	template<typename Container> double SpeedTest(const uint32_t count, size_t& sum)
	{
		vector<Container::value_type> data;
		vector<Container::value_type> lookup;

		GenerateData(count, data, lookup);

		stable_sort(data.begin(), data.end());

		Container container(data.begin(), data.end());

		const size_t repeatsNumber = 32 * 1024 * 1024 / count;

		const uint32_t start = GetTickCount();


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

	template<typename T> void SpeedTest()
	{
		std::cout << "count,vector,multimap,llookup" << std::endl;
		uint32_t index = 5;
		for (uint32_t count = 32; count <= 1024 * 1024; count *= 2)
		{
			size_t sumVector = 0;
			size_t sumMultiSet = 0;
			size_t linearLookup = 0;
			std::cout << (index++);
			std::cout << ',' << SpeedTest<vector<T> >(count, sumVector);
			std::cout << ',' << SpeedTest<multiset<T> >(count, sumMultiSet);
			std::cout << ',' << SpeedTest<LLookup<T, T> >(count, linearLookup);
			std::cout << std::endl;

		}
	}
}
