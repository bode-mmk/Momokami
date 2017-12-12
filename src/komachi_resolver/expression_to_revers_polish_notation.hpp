#ifndef KOMACHI_RESOLVER_EXPRESSION_TO_REVERSE_POLISH_NOTATION_HPP
#define KOMACHI_RESOLVER_EXPRESSION_TO_REVERSE_POLISH_NOTATION_HPP

#include <vector>
#include <stack>
#include "symbol.hpp"

namespace komachi_resolver{
	template<typename T>
	std::vector<Symbol<T>> expression_to_revers_polish_notation(const std::vector<Symbol<T>>& expression){
		// 通常の式を逆ポーランド記法にする
		std::stack<operator_symbol> operator_stack;
		std::vector<Symbol<T>> rpn;

		for(auto&& e : expression){
			if( e.has_data() ){
				rpn.emplace_back( e.get_data());
			}else{
				// 先頭のスタックと比較
				// 先頭のスタックのほうが高いか等しければスタックをおろしてくる
				if( !operator_stack.empty() && e.get_operator_symbol() <= operator_stack.top() ){
					while( !operator_stack.empty() ){
						rpn.emplace_back( operator_stack.top() );
						operator_stack.pop();
					}
				}
			operator_stack.push( e.get_operator_symbol() );
			}
		}

		if( !operator_stack.empty() ){
			while( !operator_stack.empty() ){
				rpn.emplace_back( operator_stack.top() );
				operator_stack.pop();
			}
		}
		return rpn;
	}
}

#endif
