#ifndef KOMACHI_RESOLVER_REVERSE_POLISH_NOTATION_RESOLVER_HPP
#define KOMACHI_RESOLVER_REVERSE_POLISH_NOTATION_RESOLVER_HPP

#include <vector>
#include <optional>
#include <stack>
#include "symbol.hpp"

namespace komachi_resolver{
	template<typename T>
	std::optional<T> reverse_polish_notation_resolver(const std::vector<Symbol<T>>& expression){
		// 基本的な逆ポーランド記法の演算の実装
		// wikiマン
		std::stack<T> stack;
		T accumulator = 0;

		for(auto&& e : expression){
			if( e.has_data() ){
				stack.push(e.get_data());
			}else{
				accumulator = stack.top();
				stack.pop();
				switch(e.get_operator_symbol()){
					case operator_symbol::plus :
						accumulator += stack.top();
						break;
					case operator_symbol::minus :
						accumulator = stack.top() - accumulator;
						break;
					case operator_symbol::times :
						accumulator *= stack.top();
						break;
					case operator_symbol::divide : 		
						// 0除算が発生した場合は計算失敗
						if( accumulator == 0 ){
							return std::nullopt;
						}
	
						accumulator = stack.top() / accumulator;
						break;
				}
				stack.pop();
				stack.push(accumulator);
			}
		}
	
		return std::make_optional( stack.top() );
	}
}

#endif
