#include <functional>
#include <memory>

template <class ELEM_TYPE>
class MemoryPool
{

private:
	//size_t size;
	ELEM_TYPE *ptr;
    std::unique_ptr<ELEM_TYPE> _mpool;
    std::mutex _pool_mu;
    std::unordered_map<size_t, std::deque<ELEM_TYPE*>> _slot_map;
    std::unordered_map<ELEM_TYPE*, size_t> _ptr_map;
    std::function<ELEM_TYPE*(size_t)> _alloc_fun;
    std::function<void(void*)> _free_fun;
    std::atomic_size_t _used_mem{};
public:
    MemoryPool(std::function<ELEM_TYPE*(size_t)> alloc_fun, std::function<void(void*)> free_fun);
    ~MemoryPool();
	ELEM_TYPE *alloc(size_t size);
	void free(ELEM_TYPE*);
};
