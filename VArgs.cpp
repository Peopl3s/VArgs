/*

containerToTuple - convert any  sequence container to a tuple
mapToTuple - conver map, multimap and  their hash variants to a tuple
TupleCout - print a tuple
TupleCoutMap - print a tuple containing firs and second method

MinArg - min element of sequence container
MaxArg - max element of sequence container
SumArg - sum of  element of sequence container
AvgArg - avg of sequence container
MulArg - multiplication of elements of sequence container
DiffArg - difference of elements of sequence container

*/
#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <stack>
#include <queue>
#include <tuple>
#include <cassert>
#include <type_traits>
#include <map>
#include <set>
#include <unordered_map>

namespace VArgs
 {

	template< class InputIt, class Distance >
	constexpr InputIt advanceP( InputIt it, Distance n ) 
	{
		for (std::size_t i=0; i!=n; i++) ++it;
		
		return it;
	}

	template< class InputIt, class Distance >
	constexpr auto advanceMap( InputIt it, Distance n ) 
	{
		for (std::size_t i=0; i!=n; i++) ++it;
		
		return std::pair {it->first,it->second};
	}



	template <typename U, template<typename T> class Container, std::size_t... Indices,typename std::enable_if_t<!std::is_same<Container<U>,std::queue<U>>::value && !std::is_same<Container<U>,std::stack<U>>::value && !std::is_same<Container<U>, std::priority_queue<U>>::value , std::nullptr_t> = nullptr>
	constexpr auto Impl(const Container<U>& v, std::index_sequence<Indices...>)
	 {

		return std::make_tuple(*advanceP(begin(v),Indices)...);

	}


	template <typename U, template<typename T> class Container, std::size_t... Indices,typename std::enable_if_t<std::is_same<Container<U>,std::queue<U>>::value, std::nullptr_t> = nullptr>
	constexpr auto Impl(const Container<U>& v, std::index_sequence<Indices...>) 
	{

		return std::make_tuple(advanceP(v.front(),Indices)...);

	}


	template <typename U, template<typename T> class Container, std::size_t... Indices,typename std::enable_if_t<std::is_same<Container<U>,std::stack<U>>::value ||std::is_same<Container<U>,std::priority_queue<U>>::value, std::nullptr_t> = nullptr>
	constexpr auto Impl(const Container<U>& v, std::index_sequence<Indices...>) 
	{

		return std::make_tuple(advanceP(v.top(),-Indices)...);

	}


	template <std::size_t N, typename U, template<typename T> class Container, typename std::enable_if_t<!std::is_same<Container<U>,std::forward_list<U>>::value, std::nullptr_t> = nullptr>
	auto containerToTuple(const Container<U>& v) {
		
		assert(v.size()>= N);
		
		return Impl(v,std::make_index_sequence<N>());
	}


	template <std::size_t N, typename U, template<typename T> class Container,typename std::enable_if_t<std::is_same<Container<U>,std::forward_list<U>>::value, std::nullptr_t> = nullptr>
	auto containerToTuple(const Container<U>& v) 
	{
		assert(static_cast<std::size_t>(distance(begin(v),end(v)))>= N);
		
		return Impl(v,std::make_index_sequence<N>());
	}




	template<typename Tuple, size_t...I>
	constexpr void impl_cout(Tuple&& t, std::index_sequence<I...>) 
	{

		((std::cout<<std::get<I>(std::forward<Tuple>(t))<<" "),...);

	}


	template<typename Tuple, size_t...I>
	constexpr void impl_coutMap(Tuple&& t, std::index_sequence<I...>) 
	{

		((std::cout<<std::get<I>(std::forward<Tuple>(t)).first<<" "<<std::get<I>(std::forward<Tuple>(t)).second<<" "),...);


	}


	template<typename Tuple>
	constexpr void TupleCout(Tuple&& t)
	 {
		constexpr std::size_t N=std::tuple_size<std::remove_reference_t<Tuple>>::value;
		
		impl_cout(std::forward<Tuple>(t),std::make_index_sequence<N> {});
	}


	template<typename Tuple>
	constexpr void TupleCoutMap(Tuple&& t) 
	{
		constexpr std::size_t N=std::tuple_size<std::remove_reference_t<Tuple>>::value;
		
		impl_coutMap(std::forward<Tuple>(t),std::make_index_sequence<N> {});
		
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	constexpr static  auto minI=[](const auto &f, const auto&...args) noexcept 
	{
		auto miN{f};
		
		return ((miN=miN<args?miN:args),...);
	};


	constexpr static  auto maxI=[](const auto &f, const auto&...args) noexcept
	 {
		auto maX{f};
		
		return ((maX=maX>args?maX:args),...);
	};


	constexpr static auto sumI=[](const auto&...args)noexcept
	 {
		return (args+...);
	};

	constexpr static auto diffI=[](const auto &f, const auto&...args)noexcept 
	{
		auto d{f};
		
		((d-=args),...);
		
		return d;
	};


	constexpr static auto avgI=[](const auto&...args)noexcept 
	{
		static_assert(sizeof...(args) != 0);
		
		return static_cast<double>((args+...))/sizeof...(args);
		
	};

	constexpr static auto mulI=[](const auto&...args)noexcept
	 {
		if(sizeof...(args)==0) return 0;
		
		return (args*...);
	};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t N,typename U, template<typename T> class Container>
	constexpr auto MinArg(const Container<U>&c) 
	{
		auto tup=containerToTuple<N>(c);
		
		return apply(minI,tup);
	}

	template<size_t N, typename U, template<typename T> class Container>
	constexpr auto MaxArg(const Container<U>&c) 
	{
		auto tup=containerToTuple<N>(c);
		
		return apply(maxI,tup);
	}

	template<size_t N,typename U, template<typename T> class Container>
	constexpr auto SumArg(const Container<U>&c)
	 {
		auto tup=containerToTuple<N>(c);
		
		return apply(sumI,tup);
	}


	template<size_t N, typename U, template<typename T> class Container>
	constexpr auto AvgArg(const Container<U>&c) 
	{

		if constexpr(std::is_arithmetic<U>::value)
		 {
			auto tup=containerToTuple<N>(c);
			
			return apply(avgI,tup);
		}


	}


	template<size_t N, typename U, template<typename T> class Container>
	constexpr auto MulArg(const Container<U>&c) 
	{
		assert(std::is_arithmetic<U>::value);
		
		auto tup=containerToTuple<N>(c);
		
		return apply(mulI,tup);
	}

	template<size_t N, typename U, template<typename T> class Container>
	constexpr auto DiffArg(const Container<U>&c)
	 {
		assert(std::is_arithmetic<U>::value);
		
		auto tup=containerToTuple<N>(c);
		
		return apply(diffI,tup);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename Key,typename Value, template<typename K, typename V> class Container,std::size_t... Indices>
	constexpr auto mapImpl(const Container<Key,Value>& v,std::index_sequence<Indices...>) 
	{

		return std::make_tuple(advanceMap(begin(v),Indices)...);

	}

	template <std::size_t N, typename Key,typename Value, template<typename K,typename V> class Container>
	auto mapToTuple(const Container<Key,Value>& v) 
	{

		return mapImpl(v,std::make_index_sequence<N>());

	}

}

int main() 
{
	std::vector<int>v {1,2,3,4,5,-2};
	
	std::list<int>l {1,2,3,4,5};
	
	std::forward_list<int>fl {1,2,3,4,5};
	
	std::deque<int>d {1,2,3,4,5};
	
	std::queue<int>q({1,2,3,4,5});
	
	std::stack<int>st({1,2,3,4,5});
	
	std::vector<std::string>vv {"gffg","fdfd","ddf","dffd","fddf","ffd"};

	std::priority_queue<int>pq(v.begin(),v.end());

	auto t1=VArgs::containerToTuple<6>(v);
	VArgs::TupleCout(t1);
	std::cout<<std::endl;

	auto t2=VArgs::containerToTuple<5>(l);
	VArgs::TupleCout(t2);
	std::cout<<std::endl;

	auto t3=VArgs::containerToTuple<5>(fl);
	VArgs::TupleCout(t3);
	std::cout<<std::endl;

	auto t4=VArgs::containerToTuple<5>(d);
	VArgs::TupleCout(t4);
	std::cout<<std::endl;


	auto t5=VArgs::containerToTuple<5>(q);
	VArgs::TupleCout(t5);
	std::cout<<std::endl;

	auto t6=VArgs::containerToTuple<5>(st);
	VArgs::TupleCout(t6);
	std::cout<<std::endl;


	auto t7=VArgs::containerToTuple<5>(pq);
	VArgs::TupleCout(t7);
	std::cout<<std::endl;

	std::cout<<VArgs::MinArg<6>(v);
	
	std::cout<<VArgs::MaxArg<6>(v);
	
	std::cout<<VArgs::SumArg<6>(v);
	
	std::cout<<VArgs::AvgArg<6>(v);
	
	std::cout<<VArgs::MulArg<6>(v);
	std::cout<<std::endl;
	
	std::cout<<"Difference: "<<VArgs::DiffArg<6>(v);
	std::cout<<std::endl;

	auto [f,s,t,fo,fi,ss]=VArgs::containerToTuple<6>(v);
	std::cout<<f<<" "<<s<<" "<<t<<" "<<fo<<" "<<fi<<" "<<ss<<std::endl;

	std::cout<<std::endl;
	
	std::map<int,double> m {{1,2.1},{3,4.1},{5,6.1}};
	
	std::multimap<int,double> mu {{1,2.1},{3,4.1},{5,6.1}};
	
	std::unordered_map<int,double>um {{1,2.1},{3,4.1},{5,6.1}};

	auto mapTuple=VArgs::mapToTuple<3>(m);
	VArgs::TupleCoutMap(mapTuple);
	std::cout<<std::endl;

	auto mapTuple2=VArgs::mapToTuple<3>(mu);
	VArgs::TupleCoutMap(mapTuple2);
	std::cout<<std::endl;

	auto mapTuple3=VArgs::mapToTuple<3>(um);
	VArgs::TupleCoutMap(mapTuple3);
	std::cout<<std::endl;


	std::set<int>se {1,2,3,4,5};

	auto tx=VArgs::containerToTuple<5>(se);
	VArgs::TupleCout(tx);
	std::cout<<std::endl;
	
}

