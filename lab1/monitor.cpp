#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_cond_t cond;
pthread_mutex_t mutex;
bool ready = false;

void* producer(void* arg) {
	while (true) {
		pthread_mutex_lock(&mutex);
		if (ready == true) {
			pthread_mutex_unlock(&mutex);
			continue;
		}
	
		ready = true;
		cout << "Producer: message produced" << endl;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	return nullptr;
}

void* consumer(void* arg) {
	while (true) {
		pthread_mutex_lock(&mutex);
		while (ready == false) {
			pthread_cond_wait(&cond, &mutex);
		}
		ready = false;
		cout << "Consumer: message consumed" << endl;
		pthread_mutex_unlock(&mutex);
	}

	return nullptr;
}

int main() {
	pthread_t th[2];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	pthread_create(&th[0], nullptr, producer, nullptr);
	pthread_create(&th[1], nullptr, consumer, nullptr);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	pthread_join(th[1], NULL);
	return 0;
}
