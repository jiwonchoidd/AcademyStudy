#include "TCallbackManager.h"

class IntProducer 
{
public:
	IntProducer()
		: next_callback_handle_(0) {};

	std::function<void(void)> AddCallback(std::function<void(int)> callback) 
	{
		callbacks_.emplace(next_callback_handle_, callback);
		size_t handle = next_callback_handle_;
		auto delete_callback =
			[this, handle]() {
			printf("Removing callback %zu\n", handle);
			size_t num = callbacks_.erase(handle);
			printf("Removed %zu callbacks\n", num);
		};
		++next_callback_handle_;
		return delete_callback;
	}
	void BroadcastInt(int n) 
	{
		for (auto callback : callbacks_) 
		{
			printf("Calling callback for handle %zu\n", callback.first);
			callback.second(n);
		}
	}

private:
	size_t next_callback_handle_;
	std::map<size_t, std::function<void(int)>> callbacks_;
};


int main(int argc, char** argv) 
{
	(void)argc;
	(void)argv;

	auto my_callback = std::function<void(int)>(
		[](int x) {
		printf("Received %d\n", x);
	});


	IntProducer producer;
	auto delete_print_callback = producer.AddCallback(my_callback);
	producer.BroadcastInt(3);
	producer.BroadcastInt(4);
	producer.BroadcastInt(5);
	delete_print_callback();
	producer.BroadcastInt(6);
	producer.BroadcastInt(7);

	return 0;
}