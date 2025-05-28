#pragma once

template<typename T>
class UniquePtr {
public:
	UniquePtr(const UniquePtr<T>& ptr) = delete;
	UniquePtr<T>& operator=(const UniquePtr<T>& ptr) = delete;

	explicit UniquePtr(T* ptr = nullptr) noexcept : _ptr(ptr) {}
	
	UniquePtr(UniquePtr<T>&& ptr) noexcept : _ptr(ptr._ptr){
		ptr._ptr = nullptr;
	}

	UniquePtr<T>& operator= (UniquePtr<T>&& ptr) noexcept {
		if (this != &ptr) {
			delete _ptr;
			_ptr = ptr._ptr;
			ptr._ptr = nullptr;
			return *this;
		}
	}

	T& operator*() const noexcept {
		return *_ptr; // 由用户确保 _ptr 非空
	}

	T* operator->() const noexcept {
		return _ptr; // 由用户确保 _ptr 非空
	}

	T& operator[](size_t pos) const noexcept {
		return _ptr[pos];
	}

	~UniquePtr() {
		if (_ptr) {
			delete _ptr;
			_ptr = nullptr;
		}
	}
private:
	T* _ptr;
};
