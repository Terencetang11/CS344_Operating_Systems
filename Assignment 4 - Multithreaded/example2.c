#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Size of the buffer
#define SIZE 15

// Number of items that will be produced before the END_MARKER. Note that this number is smaller than the size of the buffer. This means that we can model the buffer as unbounded
#define NUM_ITEMS 12

// Special marker used to indicate end of the producer data
#define END_MARKER -1

// Buffer, shared resource
int buffer[SIZE];
// Number of items in the buffer, shared resource
int count = 0;
// Index where the producer will put the next item
int prod_idx = 0;
// Index where the consumer will pick up the next item
int con_idx = 0;

// Initialize the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variable
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

// The buffer is unbounded. A condition variable to indicate that there is space in the buffer is no longer needed. 
// We have commented out the following line, but it can just be removed from the program.
// pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

/*
Produces a random integer between [0, 1000] unless it is the last item to be produced in which case the value -1 is returned.
*/
int produce_item(int i){
  int value;
  if (i == NUM_ITEMS)
      value = END_MARKER;
  else
      value = rand() % 1000;
  return value;
}

/*
 Put an item in the shared buffer
*/
int put_item(int value)
{
    buffer[prod_idx] = value;
    // Increment the index where the next item will be put. Roll over to the start of the buffer if the item was placed in the last slot in the buffer
    prod_idx = (prod_idx + 1) % SIZE;
    count++;
    return value;
}

/*
 Function that the producer thread will run. Produce an item and put in the buffer only if there is space in the buffer. If the buffer is full, then wait until there is space in the buffer.
*/
void *producer(void *args)
{
    for (int i = 0; i < NUM_ITEMS + 1; i++)
    {
      // Produce the item outside the critical section
      int value = produce_item(i);
      // Lock the mutex before putting item in the buffer

      pthread_mutex_lock(&mutex);

      // Since the buffer is unbounded, there is no reason to check if it is full, and if is full, then waiting for a signal from the consumer that it now has space.
      // We have commented out the following lines, but they can just be removed from the program.
      //while (count == SIZE)
      //  pthread_cond_wait(&empty, &mutex);

      put_item(value);
      // Signal to the consumer that the buffer is no longer empty
      pthread_cond_signal(&full);
      // Unlock the mutex
      pthread_mutex_unlock(&mutex);
      // Print message outside the critical section
      printf("PROD %d\n", value);
    }
    return NULL;
}

/*
 Get the next item from the buffer
*/
int get_item()
{
    int value = buffer[con_idx];
    // Increment the index from which the item will be picked up, rolling over to the start of the buffer if currently at the end of the buffer
    con_idx = (con_idx + 1) % SIZE;
    count--;
    return value;
}

/*
 Function that the consumer thread will run. Get  an item from the buffer if the buffer is not empty. If the buffer is empty then wait until there is data in the buffer.
*/
void *consumer(void *args)
{
    int value = 0;
    // Continue consuming until the END_MARKER is seen    
    while (value != END_MARKER)
    {
      // Lock the mutex before checking if the buffer has data      
      pthread_mutex_lock(&mutex);
      while (count == 0)
        // Buffer is empty. Wait for the producer to signal that the buffer has data
        pthread_cond_wait(&full, &mutex);
      value = get_item();

      // The buffer is unbounded. So there is no need to signal to the producer that the buffer has space.
      // We have commented out the following line, but it can just be removed from the program.
      // pthread_cond_signal(&empty);

      // Unlock the mutex
      pthread_mutex_unlock(&mutex);
      // Print the message outside the critical section
      printf("CONS %d\n", value);
    }
    return NULL;
}

int main()
{
    srand(time(0));
    pthread_t p, c;
    // Create the producer thread
    pthread_create(&p, NULL, producer, NULL);
    // Now create the consumer thread
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}