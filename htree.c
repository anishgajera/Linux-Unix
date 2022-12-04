#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>     // for EINTR
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

// block size
#define BSIZE 4096

void usage(char*);
uint32_t jenkins_one_at_a_time_hash(const uint8_t* , uint64_t );
//numThreads
int numThreads, block_per_threads;
char *buffer;
uint32_t total, hash;
uint64_t fileSize;
bool ar[2] = {true, false};
// Print out the usage of the program and exit.

void *hashing(void *ptr) {
        uint32_t n = (uint32_t) ptr;

        uint32_t left, right;
        pthread_t thread1, thread2;
        bool hash1 = false, hash2 = false;
        if (2 * n + 1 < numThreads) {
                pthread_create(&thread1, NULL, hashing, (void *) (2*n+1));
                hash1 = true;

                if (2 * n + 2 < numThreads) {
                        pthread_create(&thread1, NULL, hashing, (void *) (2*n+2));
                        hash1 = true;
                }
        }
        void *ptr1 = NULL, *ptr2 = NULL;
        if (hash1) {
                pthread_join(thread1, &ptr1);
        }
        if (hash2) {
                pthread_join(thread2, &ptr2);
        }
        uint32_t currentHash;
        currentHash = jenkins_one_at_a_time_hash(buffer + (n * (BSIZE*block_per_threads)), BSIZE * block_per_threads);
        char tempHash[10000];
        if(hash1 && hash2) {
                sprintf(tempHash, "%u%u%u", currentHash, ptr1, ptr2);

        } 
        else if (hash1) {
                sprintf(tempHash, "%u%u", currentHash, ptr1);
        } 
        else if (hash2) {
                sprintf(tempHash, "%u%u", currentHash, ptr2);
        } 
        else {
                return (void *) currentHash;
        }
        currentHash = jenkins_one_at_a_time_hash((uint8_t *)tempHash, strlen(tempHash));
        return (void *) currentHash;
}

double findTime() {
        struct timeval t;
        int rc = gettimeofday(&t, NULL);
        assert(rc == 0);
        return (double)t.tv_sec + (double)t.tv_sec/1e6;
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, uint64_t length)
{
  uint64_t i = 0;
  uint32_t hash = 0;

  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}


void usage(char* s)
{
  fprintf(stderr, "Usage: %s filename num_threads \n", s);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{

  int32_t fd;
  uint32_t nblocks;

  // input checking
  if (argc != 3)
    usage(argv[0]);

  // open input file
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open failed");
    exit(EXIT_FAILURE);
  }
  // use fstat to get file size
struct stat fileInfo;
  if (fstat(fd, &fileInfo)) {
        perror("File not found. ");
        exit(EXIT_FAILURE);

  }
  // calculate nblocks
  fileSize = fileInfo.st_size;
  buffer = (char *) malloc (fileSize);
        nblocks = fileSize / BSIZE;
        if (fileSize % BSIZE)
                nblocks++;
  printf("no. of blocks = %u \n", nblocks);

  uint64_t nread = 0;
  ssize_t size = 0;

  //as long as there is more data to read, keep reading until the size = 0
  while ((nread < fileSize) && ((size = read(fd, &buffer[nread], fileSize)) > 0)) {
        nread += size;
  }

  if (size < 0) {
        perror("File read error.");
        exit(EXIT_FAILURE);
  }

  pthread_t thread;
  numThreads = atoi(argv[2]);
  block_per_threads = nblocks / numThreads;
  double start = findTime();
  uint64_t num = 0;
  pthread_create(&thread, NULL, hashing, (uint32_t *) num);

  void *ptr;
  pthread_join(thread, &ptr);
  double end = findTime();
  printf("hash value = %u\n", (uint32_t) ptr);
  printf("time taken = %f \n", (end - start));
  close(fd);
  return EXIT_SUCCESS;
}
