/**
 * This file is part of OpenAtomic.
 *
 * OpenAtomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenAtomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAtomic. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PRIORITY_CONCURENT_QUEUE_H
#define PRIORITY_CONCURENT_QUEUE_H

// standard queue
#include <queue>
// boost concurency
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

using namespace std;

template<typename T>
class CPriorityConcurrentQueue
{
private:

	struct Compare_T
	{
		bool operator () (const T& left , const T& right)
		{
			return (left < right);
		}
	};

	priority_queue<T, vector<T>, Compare_T > mQueue;
	mutable boost::mutex mMutex;
	boost::condition_variable mCondVar;

public:
	void push(T const& data)
	{
		boost::mutex::scoped_lock lock(mMutex);
		mQueue.push(data);
		lock.unlock();
		mCondVar.notify_one();
	}

	bool empty() const
	{
		boost::mutex::scoped_lock lock(mMutex);
		return mQueue.empty();
	}

	bool try_pop(T& poppedValue)
	{
		boost::mutex::scoped_lock lock(mMutex);

		if(mQueue.empty())
		{
			return false;
		}

		poppedValue = mQueue.top();
		mQueue.pop();
		return true;
	}

	void wait_and_pop(T& poppedValue)
	{

		boost::mutex::scoped_lock lock(mMutex);

		while(mQueue.empty())
		{
			mCondVar.wait(lock);
		}

		poppedValue = mQueue.top();
		mQueue.pop();
	}
};

#endif
