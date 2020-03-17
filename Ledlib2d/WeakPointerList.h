#pragma once

#include <vector>
#include <memory>

namespace Ledlib {

template<typename T>
class WeakPointerList {
public:
	std::vector<std::weak_ptr<T>> items;
	WeakPointerList(){}

	bool Contains(std::shared_ptr<T> item){
		for(auto it = items.begin(); it < items.end(); it++){
			if(it->lock() == item) return true;
		}
		return false;
	}
	void Add(std::shared_ptr<T> item){
		items.push_back(item);
	}
	void Remove(std::shared_ptr<T> item){
		for(auto it = items.begin(); it < items.end(); it++){
			if(it->lock() == item) items.erase(it--);
		}
	}
	unsigned int Size(){
		return items.size();
	}
	bool HasExpired(){
		for(auto& p: items){
			if(p->expired()) return true;
		}
		return false;
	}
	void RemoveExpired(){
		for(auto it = items.begin(); it < items.end(); it++){
			if(it->expired()) items.erase(it--);
		}
	}
	void Clear(){
		items.clear();
	}

};

}

