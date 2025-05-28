#pragma once

#include <cassert>

template<typename T>
class SharedPtr {
public:
	explicit SharedPtr(T* ptr = nullptr) noexcept : _ptr(ptr) {
		if (_ptr) {
			_refCount = new int(1);
		}
	}

	SharedPtr(const SharedPtr<T>& other) noexcept : _ptr(other._ptr), _refCount(other._refCount) {
		if (_ptr) {
			++(*_refCount);
		}
	}

	SharedPtr(SharedPtr<T>&& other) noexcept : _ptr(other._ptr), _refCount(other._refCount) {
		other._ptr = nullptr;
		other._refCount = nullptr;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& other) {
		if (this != &other) {
			this->~SharedPtr();
			if (other._ptr && other._refCount) {
				_ptr = other._ptr;
				_refCount = other._refCount;
				++(*_refCount);
			}
		}
		return *this;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& other) {
		if (this != &other) {
			this->~SharedPtr();
			_ptr = other._ptr;
			_refCount = other._refCount;
			other._ptr = nullptr;
			other._refCount = nullptr;
		}
		return *this;
	}

	T& operator*() const noexcept {
		assert(ptr && "Accessing null SharedPtr");
		return *_ptr; // 由用户确保 _ptr 非空
	}

	T* operator->() const noexcept {
		assert(ptr && "Accessing null SharedPtr");
		return _ptr; // 由用户确保 _ptr 非空
	}

	T& operator[](size_t pos) const noexcept {
		assert(ptr && "Accessing null SharedPtr");
		return _ptr[pos];
	}

	T* Get() { return _ptr; }
	int UseCount() { return _refCount ? *_refCount : 0; }

	~SharedPtr() {
		if (_ptr && _refCount && !--(*_refCount)) {
			delete _ptr;
			delete _refCount;
		}
		_refCount = nullptr;
		_ptr = nullptr;
	}

private:
	T* _ptr;
	int* _refCount;
};
