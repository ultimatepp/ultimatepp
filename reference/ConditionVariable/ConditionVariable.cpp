#include <Core/Core.h>

using namespace Upp;

#define BUFFER_SIZE 10
#define PRODUCER_SLEEP_TIME_MS 500
#define CONSUMER_SLEEP_TIME_MS 2000

int   ItemBuffer[BUFFER_SIZE];

int   QueueSize;
int   QueueStartOffset;

int   TotalItemsProduced;
int   TotalItemsConsumed;

ConditionVariable ItemBufferNotEmpty;
ConditionVariable ItemBufferNotFull;
Mutex ItemBufferLock;

bool StopRequested;

void ProducerThread()
{
	LOG("Starting producer thread");
    for(;;) {
		Sleep(Random(PRODUCER_SLEEP_TIME_MS));
		{
			Mutex::Lock __(ItemBufferLock);
	
			while(QueueSize == BUFFER_SIZE && !StopRequested)
				ItemBufferNotFull.Wait(ItemBufferLock);
	
	        if(StopRequested)
	            break;
	
			int Item = Random(1000);

	        ItemBuffer[(QueueStartOffset + QueueSize) % BUFFER_SIZE] = Item;
	        QueueSize++;
	        TotalItemsProduced++;
	        
	        LOG("Producer item " << Item << ", queue size " << QueueSize);
		}
		ItemBufferNotEmpty.Signal();
    }
	LOG("Producer exiting");
}

void ConsumerThread(int id)
{
	LOG("Starting consumer thread " << id);
    for(;;)
    {
        {
            Mutex::Lock __(ItemBufferLock);

            while(QueueSize == 0 && !StopRequested)
                ItemBufferNotEmpty.Wait(ItemBufferLock);

            if(StopRequested && QueueSize == 0)
                break;

			int Item = ItemBuffer[QueueStartOffset];
			QueueSize--;
			QueueStartOffset++;
			TotalItemsConsumed++;

            if(QueueStartOffset == BUFFER_SIZE)
				QueueStartOffset = 0;

			LOG("Consumer " << id << ", item " << Item << ", queue size " << QueueSize);
        }

		ItemBufferNotFull.Signal();
        Sleep(Random(CONSUMER_SLEEP_TIME_MS));
    }

	LOG("Consumer exiting");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Thread producer, consumer[2];
	producer.Run(callback(ProducerThread));
	for(int i = 0; i < 2; i++)
		consumer[i].Run(callback1(ConsumerThread, i));

	LOG("Press enter to stop...");
    ReadStdIn();

    StopRequested = true;

	ItemBufferNotFull.Broadcast();
	ItemBufferNotEmpty.Broadcast();
	
	producer.Wait();
	for(int i = 0; i < 2; i++)
		consumer[i].Wait();

	LOG("TotalItemsProduced: " << TotalItemsProduced <<
	    ", TotalItemsConsumed: " << TotalItemsConsumed);
}
