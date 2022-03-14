#pragma once
#include <boost/lockfree/spsc_queue.hpp>
#include <thread>

namespace Utilities
{
	namespace Wrappers
	{
		template<typename T = std::byte>
		class ConcurrentQueue
		{
		public:
			ConcurrentQueue(std::size_t initialSize)
			{
				queue = std::unique_ptr<boost::lockfree::spsc_queue<T>>(new boost::lockfree::spsc_queue<T>(initialSize));
			}

			size_t ReadAvailable()
			{
				return queue->read_available();
			}

			void Enqueue(const T& val)
			{
				int counter{};

				while (!(queue->push(val) || counter == retryCount))
				{
					counter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				if (counter == retryCount)
				{
					throw std::exception{ "Enqueue operation was failed" };
				}
			}

			T& Dequeue()
			{
				int counter{};

				while (queue->read_available() == 0 && counter != retryCount)
				{
					counter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				if (counter == retryCount)
				{
					throw std::exception{ "Dequeue::read_available operation was failed, read_available is 0" };
				}

				//get the value
				T value{ queue->front() };

				counter = 0;
				while (!(queue->pop() || counter == retryCount))
				{
					counter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				if (counter == retryCount)
				{
					throw std::exception{ "Deque::pop operation was failed, read_available is 0" };
				}

				return value;
			}

			const T& Peek()
			{
				int counter{};

				while (queue->read_available() == 0 && counter != retryCount)
				{
					counter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				if (counter == retryCount)
				{
					throw std::exception{ "Peek::read_available operation was failed, read_available is 0" };
				}

				return queue->front();
			}

		private:
			//single producer single consumer wait-free queue boost::lockfree
			std::unique_ptr<boost::lockfree::spsc_queue<T>> queue;

			//Refactor
			const unsigned int retryCount = 5;
		};
	}
}


