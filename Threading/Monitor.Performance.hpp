//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

namespace MonitorPerformance
{
	class Quote
	{
	public:
		uint64_t Time;
		double Price;
		double Size;
		char Exchange[16];
		char Symbol[16];

	public:
		Quote()
		{
			ZeroMemory(this, sizeof(*this));
		}
	};
}

namespace MonitorPerformance
{
	class NoSynchCache
	{
	public:
		void Update(const Quote& quote)
		{
			m_quote = quote;
		}

		Quote Get()
		{
			return m_quote;
		}

	private:
		Quote m_quote;
	};

	class CriticalSectionCache
	{
	public:
		void Update(const Quote& quote)
		{
			CsLocker lock(m_synchronizer);
			m_quote = quote;
		}

		Quote Get()
		{
			CsLocker lock(m_synchronizer);
			return m_quote;
		}

	private:
		Quote m_quote;
		CriticalSection m_synchronizer;
	};

	class MonitorCache
	{
	public:
		void Update(const Quote& quote)
		{
			WriteLocker lock(m_synchronizer);
			m_quote = quote;
		}

		Quote Get()
		{
			ReadLocker lock(m_synchronizer);
			for (;lock.StartReading();)
			{
				Quote result = m_quote;
				if (lock.FinishReading())
				{
					return result;
				}
			}
			throw std::runtime_error("Couldn't get lock");
		}

	private:
		Quote m_quote;
		Monitor m_synchronizer;
	};

}

namespace MonitorPerformance
{
	template<typename T> class Producer
	{
	public:
		Producer(double frequency, T& cache) : m_frequency(frequency), m_cache(cache), m_profiler(64 * 1024), m_continue(false)
		{
		}

	public:
		void Start()
		{
			m_continue = true;
			Delegate<void()> func(this, &Producer::Loop);
			m_thread.Invoke(func);
		}

		void Stopping()
		{
			m_continue = false;
		}

		void Stop()
		{
			m_thread.Finalize();
		}

		ProfilerExStats GetStats() const
		{
			return m_profiler.GetStats();
		}

	private:
		void Loop()
		{
			uint64_t start = GetTickCount64();
			uint64_t generated = 0;
			double frequncy = m_frequency / 1000;

			while(m_continue)
			{
				uint64_t now = GetTickCount64();

				uint64_t interval = (now - start);
				uint64_t shouldBeGenerated = static_cast<uint64_t>(interval * frequncy);

				if (shouldBeGenerated > generated)
				{
					Generate();
					++generated;
				}
				else
				{
					Sleep(1);
				}
			}
		}

	private:
		void Generate()
		{
			Quote quote;
			ProfilerExScope scope(m_profiler);
			m_cache.Update(quote);
		}

	private:
		const double m_frequency;
		T& m_cache;
		ProfilerEx m_profiler;
		volatile bool m_continue;

	private:
		ThreadPool m_thread;
	};
}

namespace MonitorPerformance
{
	template<typename T> class Consumer
	{
	public:
		Consumer(T& cache) : m_cache(cache), m_profiler(64 * 1024)
		{
		}

	public:
		void Start()
		{
			m_continue = true;
			Delegate<void()> func(this, &Consumer::Loop);
			m_thread.Invoke(func);
		}

		void Stopping()
		{
			m_continue = false;
		}

		void Stop()
		{
			m_thread.Finalize();
		}

		ProfilerExStats GetStats() const
		{
			return m_profiler.GetStats();
		}

	private:
		void Loop()
		{
			while (m_continue)
			{
				ProfilerExScope scope(m_profiler);
				m_cache.Get();
			}
		}

	private:
		T& m_cache;
		ProfilerEx m_profiler;
		volatile bool m_continue;

	private:
		ThreadPool m_thread;
	};
}


namespace MonitorPerformance
{
	template<typename T> void RunTest(const double frequency, const uint32_t timeOfExperimentInMs, T& writeCache, T& readCache, std::ostream& stream)
	{
		MonitorPerformance::Producer<T> producer(frequency, writeCache);
		MonitorPerformance::Consumer<T> consumer(readCache);

		const uint64_t start = GetTickCount64();

		consumer.Start();
		producer.Start();

		Sleep(timeOfExperimentInMs);

		producer.Stopping();
		consumer.Stopping();

		producer.Stop();
		consumer.Stop();

		const uint64_t finish = GetTickCount64();

		double time = (finish - start) * 1000.0;

		const ProfilerExStats producerStats = producer.GetStats();
		const ProfilerExStats consumerStats = consumer.GetStats();

		stream << "frequency = " << frequency << std::endl;
		stream << "producer speed (count / mks) = " << producerStats.GetCount() / time << std::endl;
		stream << "consumer speed (count / mks) = " << consumerStats.GetCount() / time << std::endl;

		stream << "producer histogram:" << std::endl;
		producerStats.PrintHistogramInTicks(stream);
		stream << std::endl;

		stream << "consumer histogram:" << std::endl;
		consumerStats.PrintHistogramInTicks(stream);
		stream << std::endl;
	}

	template<typename T> void RunTestOnTwoCaches(const double frequency, const uint32_t timeOfExperimentInMs, std::ostream& stream)
	{
		T writeCache;
		T readCache;
		RunTest(frequency, timeOfExperimentInMs, writeCache, readCache, stream);
	}

	template<typename T> void RunTestOnOneCache(const double frequency, const uint32_t timeOfExperimentInMs, std::ostream& stream)
	{
		T cache;
		RunTest(frequency, timeOfExperimentInMs, cache, cache, stream);
	}

	void RunExperiment(const double frequency, const uint32_t timeOfExperimentInMs, std::ostream& stream)
	{
		stream << "[NO_SYNCH_2]" << std::endl;
		RunTestOnTwoCaches<NoSynchCache>(frequency, timeOfExperimentInMs, stream);
		std::cout << std::endl;

		stream << "[CRITICAL_SECTION_2]" << std::endl;
		RunTestOnTwoCaches<CriticalSectionCache>(frequency, timeOfExperimentInMs, stream);
		stream << std::endl;

		stream << "[MONITOR_2]" << std::endl;
		RunTestOnTwoCaches<MonitorCache>(frequency, timeOfExperimentInMs, stream);
		stream << std::endl;

		stream << "[NO_SYNCH_1]" << std::endl;
		RunTestOnOneCache<NoSynchCache>(frequency, timeOfExperimentInMs, stream);
		stream << std::endl;

		stream << "[CRITICAL_SECTION_1]" << std::endl;
		RunTestOnOneCache<CriticalSectionCache>(frequency, timeOfExperimentInMs, stream);
		stream << std::endl;

		stream << "[MONITOR_1]" << std::endl;
		RunTestOnOneCache<MonitorCache>(frequency, timeOfExperimentInMs, stream);
		stream << std::endl;
	}
}