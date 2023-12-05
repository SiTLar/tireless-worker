#ifndef _VECTOR_INCLUDED
#include <vector>
#endif

#ifndef _ALGORITHM_INCLUDED
#include <algorithm>
#endif
template<typename U>
struct idless: std::binary_function<U, U, bool>{
	inline bool operator()(const U& lhs, const U& rhs){ return  lhs.first< rhs.first; };
};
template<typename T>
struct  test_key: std::unary_function<std::pair<int, T>, bool>{
	const int iVictim;

	test_key(const int i): iVictim(i){};
	inline bool operator()(const std::pair<int, T> &item){
		if (item.first == iVictim) return true; 
		else return false;
	};
};

template<typename Type>
class vecid {
	public:
	typedef typename std::pair<unsigned long, Type> cont_type;
	typedef Type value_type;
	typedef typename std::vector< std::pair<unsigned long, Type> >::iterator iterator;
	protected:
	std::vector< cont_type > *Items;
	iterator itItem(const unsigned long );
	Type itemNotFound; 
	public:
	unsigned long counter;
	bool bFullCycle;

	vecid (Type);
	~vecid(); 
	const std::vector< cont_type > * getVector(){return Items;};
	int erase(unsigned long );
	unsigned long insert( const Type &); 
	Type item(unsigned long );
	Type operator[](unsigned long );

	inline iterator begin() { return Items->begin(); };
	inline iterator end() { return Items->end(); };

};



template< typename Type >
vecid< Type > :: vecid( Type inp ): itemNotFound(inp){
	Items = new std::vector<std::pair<unsigned long, Type> >;

	counter = 0;
	bFullCycle = false;
};

template<typename Type>
vecid<Type>::~vecid(){
	delete Items;
};
template<typename Type>
typename vecid<Type>::iterator vecid<Type>::itItem(unsigned long id){
	iterator itB, itE, itMid;
	if (Items->empty()) return end();
	if ((id > Items->rbegin()->first) ||(id < begin()->first) ) return end();
	itB = begin();
	if (abs(std::distance(begin(), end())) > id) itE = begin() + id;
	else itE = end();
	for(;;){
		//itMid = itE - (std::distance(itB,itE)/2);
		itMid = itB + (std::distance(itB,itE)/2);
		if (id == itMid->first) break;
		if (itB == itE) return end();
		if (id > itMid->first) itB = itMid+1;
		else itE = itMid-1;
		
	}
	return itMid;
};
/*template<class Type>
  vecid<Type>::iterator vecid<Type>::itDev(unsigned long id){
  std::pair<iterator, iterator> out = 
  equal_range (begin(), end(), 
  cont_type(id, value_type()),
  idless<cont_type>() );
  return out.first == out.second ? end() : out.first;
  };
  */
template<typename Type>
unsigned long  vecid<Type>::insert( const Type& Item){
//	if ( Items->size()  > 0xfffffff0) throw 0;
	unsigned long id;
	if (!bFullCycle){
		id = counter++;
		Items->push_back(cont_type(id, Item));
		if (counter == 0) bFullCycle = true;
	}else{
		iterator itF, itB, itE, itMid;
		itF = itB = begin();
		itE = end();
		
		if ((abs(std::distance(itF, itB))) < (itB->first)) itMid = itB;
		else for(;;){
			itMid = itE - (std::distance(itB,itE)/2);

			if (itB == itMid){
				break;
			};
			if (abs(std::distance(itF, itMid)) < itMid->first){
				if(abs(std::distance(itF,itMid-1)) == (itMid-1)->first){
					break;
				}else itE = itMid+1;
			}else itB = itMid-1;
			if (itB == itE){
				break;
			};
		}
		id = std::distance(itF, itMid);
		Items->insert(itMid, cont_type(id, Item));
	}
	return id;
};

template<typename Type>
int vecid<Type>::erase(unsigned long id){
	iterator it(itItem(id));
	if (it != end())
		Items->erase(it);
	return Items->size();
};


template<typename Type>
Type vecid<Type>::item(unsigned long id){
	iterator it = itItem(id);
	if (it == end()) return itemNotFound;
	else return it->second;
};
template<typename Type>
Type vecid<Type>::operator[](unsigned long id){
	iterator it = itItem(id);
	if (it == end()) return itemNotFound;
	else return it->second;
};
