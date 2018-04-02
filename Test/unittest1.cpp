#include "stdafx.h"
#include "CppUnitTest.h"

#include <ctime>
#include <vector>

#include "../src/BufferManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(AllocateAndRelease)
		{
			const size_t buffers = 100;
			BufferManager<512, buffers> Manager;

			for (int i = 0; i < buffers; ++i)
			{
				auto ptr = Manager.GetBuffer();
				Manager.ReleaseBuffer(ptr);
			}
		}	
		
		TEST_METHOD(AllocateAllAndRelease)
		{
			const size_t buffers = 100;
			BufferManager<512, buffers> Manager;

			std::vector<uint8_t*> allocatedBuffers;
			
			try {
				for (int i = 0; i < buffers; ++i)
				{
					auto ptr = Manager.GetBuffer();
					allocatedBuffers.push_back(ptr);
				}
			}
			catch (const std::bad_alloc& a)
			{
				Assert::Fail(L"All buffers not allocated");
			}

			bool ex_catch = false;
	
			try {
				Manager.GetBuffer();
			}
			catch( const std::bad_alloc& )
			{
				ex_catch = true;
			}

			if (!ex_catch)
			{
				Assert::Fail(L"All buffers not allocated");
			}

			for (int i = 0; i < buffers; ++i)
			{
				Manager.ReleaseBuffer(allocatedBuffers[i]);
			}

		}
		
		TEST_METHOD(AllocateAllAndReleaseRandom)
		{
			srand(time(NULL));

			const size_t buffers = 100;
			BufferManager<512, buffers> Manager;

			std::vector<uint8_t*> allocatedBuffers;

			try {
				for (int i = 0; i < buffers; ++i)
				{
					auto ptr = Manager.GetBuffer();
					allocatedBuffers.push_back(ptr);
				}
			}
			catch (const std::bad_alloc& a)
			{
				Assert::Fail(L"All buffers not allocated");
			}

			bool ex_catch = false;

			try {
				Manager.GetBuffer();
			}
			catch (const std::bad_alloc&)
			{
				ex_catch = true;
			}

			if (!ex_catch)
			{
				Assert::Fail(L"All buffers not allocated");
			}
		
			int random_range = buffers;
			for (int i = 0; i < buffers; ++i)
			{
				int rand_index = rand() % random_range;

				Manager.ReleaseBuffer(allocatedBuffers[rand_index]);
				allocatedBuffers.erase(allocatedBuffers.begin() + rand_index);

				random_range--;
			}

		}

	};
}